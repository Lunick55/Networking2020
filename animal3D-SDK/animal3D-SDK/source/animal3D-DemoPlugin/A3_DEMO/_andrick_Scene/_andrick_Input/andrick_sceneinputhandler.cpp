#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

SceneInputHandler::SceneInputHandler(const unsigned int linesToDisplay) :
	mpChatLog(std::make_unique<ChatLog>(linesToDisplay)) {}

void SceneInputHandler::clear()
{
	mCurrentInput.clear();
	mpChatLog->clearChatLog();
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

void SceneInputHandler::getInput()
{
	if (!handleBackspace(mCurrentInput))
	{
		handleTyping(mCurrentInput);
	}
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