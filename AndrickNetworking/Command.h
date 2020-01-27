#ifndef COMMAND_H_
#define COMMAND_H_

#include "Common.h"

//The server uses this, the clients send command packets to the server.
class Command
{
public:
	//Returns true if the user has the right authority level.
	bool canExecute();

private:
	PacketEventId mId;
	AuthorityId mMinimumAuthority;
};

//Requried
//SetAuthorityCommand
//PrivateMessageCommand
//PublicMessageCommand

//Extras
//MuteCommand
//UnmuteCommand
//KickCommand
//BanCommand
//UnbanCommand

#pragma pack(push, 1)
//Base for all command events
struct CommandPacket
{
	PacketEventId packetId;

	UserId userWhoExecutedCommand;

	CommandPacket(PacketEventId id, UserId user) :
		packetId(id), userWhoExecutedCommand(user)
	{}
};

struct SetAuthorityCommandPacket : public CommandPacket
{
	AuthorityId newAuthorityId;
	UserId userToSetAuthority;

	SetAuthorityCommandPacket(UserId userThatExecuted, UserId affectedUser, AuthorityId newAuth) :
		CommandPacket(PacketEventId::SET_AUTHORITY, userThatExecuted),
		newAuthorityId(newAuth), userToSetAuthority(affectedUser) 
	{}
};

struct MessageCommandPacket : public CommandPacket
{
	char message[sMAX_MESSAGE_LENGTH];

	//Defaults to public message
	MessageCommandPacket(UserId userThatExecuted, const std::string& m) :
		MessageCommandPacket(PacketEventId::SEND_PUBLIC_MESSAGE, userThatExecuted, m)
	{}

protected:
	MessageCommandPacket(PacketEventId id, UserId userThatExecuted, const std::string& m) :
		CommandPacket(id, userThatExecuted)
	{
		strcpy(message, m.c_str());
	}
};

struct PrivateMessageCommandPacket : public MessageCommandPacket
{
	UserId userToSendTo;

	PrivateMessageCommandPacket(UserId userThatExecuted, UserId receiver, const std::string& m) :
		MessageCommandPacket(PacketEventId::SEND_PRIVATE_MESSAGE, userThatExecuted, m),
		userToSendTo(receiver)
	{}
};

struct MuteCommandPacket : public CommandPacket
{
	UserId affectedUser;

	//Defaults to mute command
	MuteCommandPacket(UserId userThatExecuted, UserId mutedUser) :
		MuteCommandPacket(PacketEventId::MUTE_USER, userThatExecuted, mutedUser)
	{}

protected:
	MuteCommandPacket(PacketEventId id, UserId userThatExecuted, UserId mutedUser) :
		CommandPacket(id, userThatExecuted),
		affectedUser(mutedUser)
	{}
};

struct UnmuteCommandPacket : public MuteCommandPacket
{
	UnmuteCommandPacket(UserId userThatExecuted, UserId unmutedUser) :
		MuteCommandPacket(PacketEventId::MUTE_USER, userThatExecuted, unmutedUser)
	{}
};
#pragma pack(pop)

#endif