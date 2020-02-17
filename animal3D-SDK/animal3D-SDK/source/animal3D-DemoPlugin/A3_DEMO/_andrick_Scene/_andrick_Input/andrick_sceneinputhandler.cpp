#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Event/TestEvent.h>

SceneInputHandler::SceneInputHandler() :
	mpChatLog(std::make_shared<ChatLog>()) {}

void SceneInputHandler::clear()
{
	clearCurrentInput();
	mpChatLog->clearChatLog();
}

void SceneInputHandler::clearCurrentInput()
{
	mCurrentInput.clear();
}

bool SceneInputHandler::isCommand(const std::string& input)
{
	return input[0] == _COMMAND_DELIMITER;
}

bool SceneInputHandler::processCommand(const std::string& input, std::shared_ptr<struct Command>& out)
{
	return Command::processCommand(input.substr(1), out);
	//if (commandInput.compare(LIST_USERS_COMMAND) == 0)
	//{
	//	success = processListCommand();
	//	return success;
	//}
	//std::size_t spaceIndex = input.find_first_of(' ');
	//std::string commandWithSpace = input.substr(1, spaceIndex);
	//std::string commandArgs = input.substr(commandWithSpace.size() + 1);
	//commandInput = commandWithSpace.substr(0, commandWithSpace.size() - 1);
	//if (commandInput.compare(WHISPER_COMMAND) == 0)
	//{
	//	success = WhisperCommand::createCommand(commandArgs, command);
	//}
	//else if (commandInput.compare(START_GAME_COMMAND) == 0)
	//{
	//	success = processStartGameCommand(commandArgs, outputResults);
	//}
	//else if (commandInput.compare(SELECT_PLAYERS_COMMAND) == 0)
	//{
	//	success = processSelectPlayersCommand(commandArgs, outputResults);
	//}
	//else if (commandInput.compare(PLAY_TURN_COMMAND) == 0)
	//{
	//	success = processPlayTictacCommand(commandArgs, outputResults);
	//}
	//return success;
}

std::shared_ptr<ChatLog> SceneInputHandler::getChatLog()
{
	return mpChatLog;
}

bool SceneInputHandler::isKeyPressed(enum a3_KeyboardKey key)
{
	return a3keyboardIsChangedASCII(gDemoState->keyboard, key) > 0;
}

bool SceneInputHandler::isKeyHeld(enum a3_KeyboardKey key)
{
	return a3keyboardIsHeldASCII(gDemoState->keyboard, key);
}

bool SceneInputHandler::isKeyReleased(enum a3_KeyboardKey key)
{
	return a3keyboardIsReleasedASCII(gDemoState->keyboard, key);
}

void SceneInputHandler::updateInput()
{
	if (!handleBackspace(mCurrentInput))
	{
		handleTyping(mCurrentInput);
	}
}

bool SceneInputHandler::handleCommandInput()
{
	return handleCommandInput(mCurrentInput);
}

bool SceneInputHandler::isCommand()
{
	return isCommand(mCurrentInput);
}

bool SceneInputHandler::processCommand(std::shared_ptr<struct Command>& out)
{
	return processCommand(mCurrentInput, out);
}

bool SceneInputHandler::handleCommandInput(const std::string& input)
{
	bool success = isCommand(input);

	if (success)
	{
		std::shared_ptr<Command> out;
		if (processCommand(input, out))
		{
			std::shared_ptr<CommandEvent> evnt;

			switch (out->ID)
			{
			case CommandId::WHISPER:
				evnt = std::make_shared<WhisperCommandEvent>(std::dynamic_pointer_cast<WhisperCommand>(out));
				break;
			default:
				std::cout << "I don't know how to process commad with ID: " << std::to_string((int)out->ID) << "!" << std::endl;
				return false;
			}

			gEventSystem.queueLocalEvent(evnt);//For now for testing. Remove later
			gEventSystem.queueNetworkEvent(evnt);
		}
	}

	return success;
}

const std::string& SceneInputHandler::getCurrentInput() const
{
	return mCurrentInput;
}

bool SceneInputHandler::handleBackspace(std::string& input)
{
	bool success = a3keyboardIsPressed(gDemoState->keyboard, a3key_backspace) && input.size() > 0;
	if (success)
	{
		input = input.substr(0, input.size() - 1);
	}

	return success;
}

void SceneInputHandler::handleTyping(std::string& input)
{
	input += gDemoState->newInput;
}

bool SceneInputHandler::validateUsername(std::string& input)
{
	return (input.length() <= sMAX_USERNAME_LENGTH
		&& (input.length() >= 1)
		&& (input.find(' ') == std::string::npos)
		&& (input.find(',') == std::string::npos));
}

bool SceneInputHandler::validateNumber(const std::string& input, int& number)
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