#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>


//////////////////////////////////////////////////
// COMMAND
//////////////////////////////////////////////////

bool Command::compareWithVector(const std::string& input, const std::vector<std::string>& vector)
{
	bool success = false;

	for (std::vector<std::string>::const_iterator iter = _LIST_MODS.begin(); iter != _LIST_MODS.end(); ++iter)
	{
		if (input.compare(iter->c_str()) == 0)
		{
			success = true;
			break;
		}
	}

	return success;
}

const std::string& Command::mergeCommand(const std::vector<std::string>& commandArgs, std::size_t startIndex)
{
	std::stringstream ss;
	for (std::vector<std::string>::const_iterator iter = commandArgs.begin() + startIndex; iter < commandArgs.end(); ++iter)
	{
		ss << iter->c_str() << " ";
	}

	return ss.str();
}

bool Command::splitCommand(const std::string& userInput, std::vector<std::string>& out)
{
	std::istringstream ss(userInput);

	do 
	{
		std::string word;
		ss >> word;

		if (!word.empty())
		{
			out.push_back(word);
		}

	} while (ss);

	return out.size() > 0;
}

//			/command arg1 arg2 ...
bool Command::processCommand(const std::string& userInput, std::shared_ptr<Command>& out)
{
	std::vector<std::string> commandArgs;
	bool success = splitCommand(userInput.substr(1), commandArgs);

	if (success)
	{
		const std::string command = commandArgs.at(0);

		if (command.compare(COMMAND_LIST) == 0)
		{
			success = ListCommand::createCommand(commandArgs, out);
		}
		else if (command.compare(COMMAND_WHISPER) == 0)
		{
			success = WhisperCommand::createCommand(commandArgs, out);
		}
	}

	return success;
}


//////////////////////////////////////////////////
// LIST
//////////////////////////////////////////////////

//				/list mods - min of two
bool ListCommand::createCommand(const std::vector<std::string>& commandArgs, std::shared_ptr<Command>& out)
{
	//Invalid input
	if (commandArgs.size() < 2)
	{
		out = nullptr;
		return false;
	}

	const std::string command = commandArgs.at(1);
	ListType type = ListType::INVALID_TYPE;

	if (command.compare(_LIST_ALL) == 0)
	{
		type = ListType::ALL;
	}
	else if (compareWithVector(command, _LIST_MODS))
	{
		type = ListType::MODS;
	}
	else if (compareWithVector(command, _LIST_PLAYERS))
	{
		type = ListType::PLAYERS;
	}
	else
	{
		out = nullptr;
		return false;
	}

	out = std::make_shared<ListCommand>(gDemoState->mpUser, ListType::ALL);
	return true;
}

ListCommand::ListCommand(class std::shared_ptr<class User> sender, ListType type) :
	Command(CommandId::LIST_PLAYERS),
	mpSender(sender),
	mType(type) {}

//////////////////////////////////////////////////
// WHISPER
//////////////////////////////////////////////////

//				/whisper Andy608 hi - min of three 
bool WhisperCommand::createCommand(const std::vector<std::string>& commandArgs, std::shared_ptr<Command>& out)
{
	//Invalid input
	if (commandArgs.size() < 3)
	{
		out = nullptr;
		return false;
	}

	std::string receiver = commandArgs.at(1);
	std::string message = mergeCommand(commandArgs, 2);
	out = std::make_shared<WhisperCommand>(gDemoState->mpUser, receiver, message);
	return true;

}

WhisperCommand::WhisperCommand(std::shared_ptr<User> sender, std::shared_ptr<Client> reciever,
	const std::string& message) :
	Command(CommandId::WHISPER),
	mpSender(sender),
	mpReciever(reciever),
	mMessage(message) {}