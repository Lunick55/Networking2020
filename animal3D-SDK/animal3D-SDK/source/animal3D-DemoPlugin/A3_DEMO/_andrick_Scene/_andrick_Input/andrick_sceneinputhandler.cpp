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

bool SceneInputHandler::validateIPAddress(const std::string& input, std::string& output)
{
	output = "";
	std::vector<std::string> list = splitString(input, '.');

	if (list.size() != 4)
	{
		return false;
	}

	for (std::string ipToken : list)
	{
		int num = 0;
		if (!validateNumber(ipToken, num) || stoi(ipToken) > 255 || stoi(ipToken) < 0)
		{
			return false;
		}
	}

	output = input;
	return true;
}

bool SceneInputHandler::validateNumber(const std::string& input, int& number)
{
	try
	{
		number = std::stoi(input);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

std::vector<std::string> SceneInputHandler::splitString(const std::string& input, char delimiter)
{
	std::size_t index = 0;
	std::vector<std::string> list;

	std::size_t pos = input.find(delimiter);

	while (pos != std::string::npos)
	{
		list.push_back(input.substr(index, pos - index));
		index = ++pos;
		pos = input.find(delimiter, pos);
	}

	list.push_back(input.substr(index, input.length()));

	return list;
}