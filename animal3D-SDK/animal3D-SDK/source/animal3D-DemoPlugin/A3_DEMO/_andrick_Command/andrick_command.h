#ifndef COMMAND_H_
#define COMMAND_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

//Constants
static const unsigned char _COMMAND_DELIMITER = '/';
static const std::string COMMAND_WHISPER = "whisper";
static const std::string COMMAND_LIST = "list";
static const std::string _LIST_ALL = "all";
static const std::vector<std::string> _LIST_MODS = { "mod", "mods", "moderators" };
static const std::vector<std::string> _LIST_PLAYERS = {"player", "players", "client", "clients" };

enum class ListType : char
{
	INVALID_TYPE = -1,
	ALL,
	MODS,
	PLAYERS
};

//static const std::string START_GAME_COMMAND = "game";
//static const std::string PLAY_TURN_COMMAND = "play";
//static const std::string SELECT_PLAYERS_COMMAND = "players";

//Parent command struct
struct Command
{
	static bool processCommand(const std::string& userInput, std::shared_ptr<Command>& out);
	static bool splitCommand(const std::string& userInput, std::vector<std::string>& out);
	static const std::string mergeCommand(const std::vector<std::string>& commandArgs, std::size_t startIndex);
	static bool compareWithVector(const std::string& input, const std::vector<std::string>& vector);

	Command(CommandId id) : ID(id) {}
	virtual ~Command() = default;

	virtual void runCommand() = 0;

	const CommandId ID;
};

//List command data
struct ListCommand : public Command
{
	static bool createCommand(const std::vector<std::string>& commandArgs, std::shared_ptr<Command>& out);

	ListCommand(class std::shared_ptr<class Client> sender, ListType type);
	virtual ~ListCommand() = default;

	virtual void runCommand();

	std::shared_ptr<class Client> mpSender;
	ListType mType;
};

//Whisper command data
struct WhisperCommand : public Command
{
	static bool createCommand(const std::vector<std::string>&, std::shared_ptr<Command>& out);

	WhisperCommand(class std::shared_ptr<class Client> sender, class std::shared_ptr<class Client> reciever,
		const std::string& message);
	virtual ~WhisperCommand() = default;

	virtual void runCommand();

	std::shared_ptr<class Client> mpSender;
	std::shared_ptr<class Client> mpReciever;
	std::string mMessage;
};

#endif