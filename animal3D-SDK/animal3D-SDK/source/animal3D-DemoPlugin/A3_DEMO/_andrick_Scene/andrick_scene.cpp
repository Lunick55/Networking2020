#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
//#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
//#include "A3_DEMO/_andrick_Network/andrick_client.h"
//#include "A3_DEMO/_andrick_Network/andrick_host.h"

Scene::Scene(const SceneId id, const Color backgroundColor) :
	mId(id),
	mBackgroundColor(backgroundColor),
	mpInputHandler(std::make_unique<SceneInputHandler>())
{

}

const SceneId Scene::getId() const 
{ 
	return mId; 
}

void Scene::enteringScene()
{
	mpInputHandler->clear();
}

void Scene::input()
{
	mpInputHandler->processInput();
}

void Scene::processIncomingEvents()
{
	
}

void Scene::packageOutgoingEvents()
{

}

void Scene::render()
{
	glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
}

//bool Scene::isCommand(const std::string& input)
//{
//	return input[0] == '/';
//}

//bool Scene::processCommand(const std::string& input, std::string& commandOutput, std::string& outputResults)
//{
//	bool success = false;
//	std::string command = input.substr(1);
//
//	if (command.compare(LIST_USERS_COMMAND) == 0)
//	{
//		commandOutput = LIST_USERS_COMMAND;
//		success = processListCommand();
//		return success;
//	}
//
//	std::size_t spaceIndex = input.find_first_of(' ');
//	std::string commandWithSpace = input.substr(1, spaceIndex);
//	std::string commandArgs = input.substr(commandWithSpace.size() + 1);
//
//	command = commandWithSpace.substr(0, commandWithSpace.size() - 1);
//
//	if (command.compare(WHISPER_COMMAND) == 0)
//	{
//		commandOutput = WHISPER_COMMAND;
//		success = processWhisperCommand(commandArgs);
//	}
//	else if (command.compare(START_GAME_COMMAND) == 0)
//	{
//		commandOutput = START_GAME_COMMAND;
//		success = processStartGameCommand(commandArgs, outputResults);
//	}
//	else if (command.compare(SELECT_PLAYERS_COMMAND) == 0)
//	{
//		commandOutput = SELECT_PLAYERS_COMMAND;
//		success = processSelectPlayersCommand(commandArgs, outputResults);
//	}
//	else if (command.compare(PLAY_TURN_COMMAND) == 0)
//	{
//		commandOutput = PLAY_TURN_COMMAND;
//		success = processPlayTictacCommand(commandArgs, outputResults);
//	}
//
//	return success;
//}

//bool Scene::processPlayTictacCommand(const std::string& commandArgs, std::string& outputResults)
//{
//	bool success = commandArgs.size() == 1;
//
//	if (success)
//	{
//		outputResults = commandArgs[0];
//	}
//
//	return success;
//}

//bool Scene::processWhisperCommand(const std::string& commandArgs)
//{
//	std::string receiver;
//	std::string message;
//	std::size_t commaIndex = commandArgs.find_first_of(',');
//
//	if ((commaIndex + 1) < commandArgs.size())
//	{
//		receiver = commandArgs.substr(0, commaIndex);
//		message = commandArgs.substr(commaIndex + 1);
//
//		if (Client::isHost())
//		{
//			Host::spInstance->deliverPersonalMessage(receiver, message);
//		}
//		else
//		{
//			Client::spInstance->sendPrivateMessageRequest(message, receiver);
//		}
//
//		return true;
//	}
//
//	addToChatList(MessageType::EITHER, "Command invalid.");
//	return false;
//}

//bool Scene::processStartGameCommand(const std::string& commandArgs, std::string& outputResults)
//{
//	if (Client::isHost())
//	{
//		if (commandArgs.find(PLAY_TICTACTOE) != std::string::npos)
//		{
//			outputResults = PLAY_TICTACTOE;
//			return true;
//		}
//		else if (commandArgs.find(PLAY_BATTLE) != std::string::npos)
//		{
//			outputResults = PLAY_BATTLE;
//			return true;
//		}
//		else
//		{
//			addToChatList(MessageType::EITHER, "Enter a valid game next time!");
//			return false;
//		}
//	}
//
//	addToChatList(MessageType::EITHER, "You don't have permission to use this command!");
//	return false;
//}

//bool Scene::processListCommand()
//{
//	if (Client::isHost())
//	{
//		Host::listUserInfoRequest();
//		return true;
//	}
//	else
//	{
//		addToChatList(MessageType::EITHER, "You don't have permission to use this command!", 1, AndrickColors::RED);
//		return false;
//	}
//}

//bool Scene::processSelectPlayersCommand(const std::string& commandArgs, std::string& outputResults)
//{
//	if (Client::isHost())
//	{
//		const static int OPPONENT_COUNT = 2;
//		std::string playerNames[OPPONENT_COUNT];
//		std::size_t commaIndex = commandArgs.find_first_of(',');
//
//		if ((commaIndex + 2) < commandArgs.size())
//		{
//			playerNames[0] = commandArgs.substr(0, commaIndex);
//			playerNames[1] = commandArgs.substr(commaIndex + 2);
//		}
//
//		int playersFound = 0;
//		for (int i = 0; i < OPPONENT_COUNT; ++i)
//		{
//			std::map<UserId, std::shared_ptr<User>>::iterator iter = Host::spInstance->mpConnectedUsers.begin();
//			for (; iter != Host::spInstance->mpConnectedUsers.end(); ++iter)
//			{
//				if (iter->second->getUsername().compare(playerNames[i]) == 0)
//				{
//					++playersFound;
//				}
//			}
//		}
//
//		if (playersFound == OPPONENT_COUNT)
//		{
//			addToChatList(MessageType::EITHER, "Found players!", 1, AndrickColors::GREEN);
//			outputResults = playerNames[0] + "," + playerNames[1];
//			return true;
//		}
//
//		addToChatList(MessageType::EITHER, "Couldn't find one or more of the players entered.", 1, AndrickColors::RED);
//		return false;
//	}
//	else
//	{
//		addToChatList(MessageType::EITHER, "You don't have permission to use this command!", 1, AndrickColors::RED);
//		return false;
//	}
//
//}

void Scene::sendPublicMessage(const std::string& message, MessageType type)
{
	//if (Client::isHost())
	//{
	//	Host::spInstance->deliverPublicMessage(Host::spInstance->mpHost, mCurrentInput, type);
	//}
	//else
	//{
	//	Client::spInstance->sendPublicMessage(mCurrentInput);
	//}
}
