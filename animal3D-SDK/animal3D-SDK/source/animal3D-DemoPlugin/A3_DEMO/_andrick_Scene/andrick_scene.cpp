#include "andrick_scene.h"
#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "A3_DEMO/_andrick_Network/andrick_client.h"
#include "A3_DEMO/_andrick_Network/andrick_host.h"

Scene::Scene(const SceneId id) :
	mId(id)
{

}

const SceneId Scene::getId() const 
{ 
	return mId; 
}

void Scene::enteringScene(const a3_DemoState* demoState)
{
	mCurrentInput.clear();
	mChatLog.clear();
}

void Scene::addToChatList(MessageType type, const std::string& msg, unsigned int newLineAmount, a3vec4 color)
{
	LogInfo info;
	info.type = type;
	info.text = msg;
	info.color = color;
	addToChatList(info, newLineAmount);
}

void Scene::addToChatList(LogInfo logInfo, unsigned int newLineAmount)
{
	mChatLog.emplace_back(logInfo);

	LogInfo spacer;
	spacer.text = "\n";
	spacer.type = logInfo.type;

	unsigned int i = 0;
	while (i < newLineAmount)
	{
		i++;
		mChatLog.emplace_back(spacer);
	}
}

bool Scene::isKeyPressed(const a3_DemoState* demoState, a3_KeyboardKey key)
{
	return a3keyboardIsChangedASCII(demoState->keyboard, key) > 0;
}

bool Scene::isKeyHeld(const a3_DemoState* demoState, a3_KeyboardKey key)
{
	return a3keyboardIsHeldASCII(demoState->keyboard, key);
}

bool Scene::isKeyReleased(const a3_DemoState* demoState, a3_KeyboardKey key)
{
	return a3keyboardIsReleasedASCII(demoState->keyboard, key);
}

void Scene::input(a3_DemoState* demoState)
{
	if (!handleBackspace(demoState, mCurrentInput))
	{
		handleTyping(demoState, mCurrentInput);
	}
}

bool Scene::handleBackspace(const a3_DemoState* demoState, std::string& input)
{
	bool success = a3keyboardIsPressed(demoState->keyboard, a3key_backspace) && input.size() > 0;
	if (success)
	{
		input = input.substr(0, input.size() - 1);
	}

	return success;
}

void Scene::handleTyping(const a3_DemoState* demoState, std::string& input)
{
	input += demoState->newInput;
}

void Scene::networkReceive(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		Host::spInstance->update(demoState);
	}
	else
	{
		Client::spInstance->update(demoState);
	}
}

void Scene::networkSend(const a3_DemoState* demoState)
{

}

bool Scene::isCommand(const std::string& input)
{
	return input[0] == '/';
}

bool Scene::processCommand(const a3_DemoState* demoState, const std::string& input, std::string& commandOutput, std::string& outputResults)
{
	bool success = false;
	std::string command = input.substr(1);

	if (command.compare(LIST_USERS_COMMAND) == 0)
	{
		commandOutput = LIST_USERS_COMMAND;
		success = processListCommand(demoState);
		return success;
	}

	std::size_t spaceIndex = input.find_first_of(' ');
	std::string commandWithSpace = input.substr(1, spaceIndex);
	std::string commandArgs = input.substr(commandWithSpace.size());

	command = commandWithSpace.substr(0, commandWithSpace.size() - 1);

	if (command.compare(WHISPER_COMMAND) == 0)
	{
		commandOutput = WHISPER_COMMAND;
		success = processWhisperCommand(demoState, commandArgs);
	}
	else if (command.compare(START_GAME_COMMAND) == 0)
	{
		commandOutput = START_GAME_COMMAND;
		success = processStartGameCommand(demoState, commandArgs, outputResults);
	}
	else if (command.compare(SELECT_PLAYERS_COMMAND) == 0)
	{
		commandOutput = SELECT_PLAYERS_COMMAND;
		success = processSelectPlayersCommand(demoState, commandArgs, outputResults);
	}
	else if (command.compare(PLAY_TURN_COMMAND) == 0)
	{
		commandOutput = PLAY_TURN_COMMAND;
		success = processPlayTictacCommand(demoState, commandArgs, outputResults);
	}

	return success;
}

bool Scene::processPlayTictacCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults)
{
	bool success = commandArgs.size() == 1;

	if (success)
	{
		outputResults = commandArgs[0];
	}

	return success;
}


bool Scene::processWhisperCommand(const a3_DemoState* demoState, const std::string& commandArgs)
{
	std::string receiver;
	std::string message;
	std::size_t commaIndex = commandArgs.find_first_of(',');

	if ((commaIndex + 1) < commandArgs.size())
	{
		receiver = commandArgs.substr(0, commaIndex);
		message = commandArgs.substr(commaIndex + 1);

		if (Client::isHost())
		{
			Host::spInstance->deliverPersonalMessage(demoState, receiver, message);
		}
		else
		{
			Client::spInstance->sendPrivateMessageRequest(message, receiver);
		}

		return true;
	}

	addToChatList(MessageType::EITHER, "Command invalid.");
	return false;
}

bool Scene::processStartGameCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults)
{
	if (Client::isHost())
	{
		if (commandArgs.find(PLAY_TICTACTOE) != std::string::npos)
		{
			outputResults = PLAY_TICTACTOE;
			return true;
		}
		else if (commandArgs.find(PLAY_BATTLE) != std::string::npos)
		{
			outputResults = PLAY_BATTLE;
			return true;
		}
		else
		{
			addToChatList(MessageType::EITHER, "Enter a valid game next time!");
			return false;
		}
	}

	addToChatList(MessageType::EITHER, "You don't have permission to use this command!");
	return false;
}

bool Scene::processListCommand(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		Host::listUserInfoRequest(demoState);
		return true;
	}
	else
	{
		addToChatList(MessageType::EITHER, "You don't have permission to use this command!", 1, TextFormatter::RED);
		return false;
	}
}

bool Scene::processSelectPlayersCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults)
{
	if (Client::isHost())
	{
		const static int OPPONENT_COUNT = 2;
		std::string playerNames[OPPONENT_COUNT];
		std::size_t commaIndex = commandArgs.find_first_of(',');

		if ((commaIndex + 1) < commandArgs.size())
		{
			playerNames[0] = commandArgs.substr(0, commaIndex);
			playerNames[1] = commandArgs.substr(commaIndex + 1);
		}

		int playersFound = 0;
		for (int i = 0; i < OPPONENT_COUNT; ++i)
		{
			std::map<UserId, std::shared_ptr<User>>::iterator iter = Host::spInstance->mpConnectedUsers.begin();
			for (; iter != Host::spInstance->mpConnectedUsers.end(); ++iter)
			{
				if (iter->second->getUsername().compare(playerNames[i]) == 0)
				{
					++playersFound;
				}
			}
		}

		if (playersFound == OPPONENT_COUNT)
		{
			addToChatList(MessageType::EITHER, "Found players!", 1, TextFormatter::GREEN);
			outputResults = playerNames[0] + "," + playerNames[1];
			return true;
		}

		addToChatList(MessageType::EITHER, "Couldn't find one or more of the players entered.", 1, TextFormatter::RED);
		return false;
	}
	else
	{
		addToChatList(MessageType::EITHER, "You don't have permission to use this command!", 1, TextFormatter::RED);
		return false;
	}

}

void Scene::sendPublicMessage(const a3_DemoState* demoState, const std::string& message, MessageType type)
{
	if (Client::isHost())
	{
		Host::spInstance->deliverPublicMessage(demoState, Host::spInstance->mpHost, mCurrentInput, type);
	}
	else
	{
		Client::spInstance->sendPublicMessage(mCurrentInput);
	}
}

bool Scene::validateUsername(std::string& input)
{
	return (input.length() <= sMAX_USERNAME_LENGTH 
		&& (input.find(' ') == std::string::npos) 
		&& (input.find(',') == std::string::npos));
}

bool Scene::validateNumber(const std::string& input, int& number)
{
	try
	{
		number = std::stoi(mCurrentInput);
		return true;
	}
	catch (...)
	{
		return false;
	}
}