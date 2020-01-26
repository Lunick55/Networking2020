#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <memory>
#include <string>

#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>

typedef unsigned char UserId;

static const unsigned char DELIMITER = '*';
static const int sMAX_USERNAME_LENGTH = 20;
static const int sMAX_USERS = 64;
static const int sMAX_MESSAGE_LENGTH = 128;
static const int sDEFAULT_MAX_USERS = 10;
static const int sPORT = 5555;

static const HANDLE windowHandle = GetStdHandle(STD_OUTPUT_HANDLE);
static const int MAX_INPUT_LENGTH = 512;
static const int ENTER_KEY = 13;
static const int TAB_KEY = 9;
static const int ESC_KEY = 27;
static const int BACKSPACE_KEY = 8;

const enum Keys { ONE = 49, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };

const enum class SceneId { MAIN_MENU, JOIN_CHATROOM, CREATE_CHATROOM, CHATROOM };

enum class AuthorityId : unsigned char
{
	NORMAL,
	MODERATOR,
	ADMIN
};

enum PacketEventId : unsigned char
{
	SET_AUTHORITY = ID_USER_PACKET_ENUM,

	SEND_PUBLIC_MESSAGE,		//Clientside command
	SEND_PRIVATE_MESSAGE,		//Clientside command

	DELIVER_PUBLIC_MESSAGE,		//Serverside command
	DELIVER_PRIVATE_MESSAGE,	//Serverside command

	REQUEST_JOIN_SERVER,		//Client asks server to join
	JOIN_ACCEPTED,				//Server sends a welcome message to joined user
	USER_JOINED_SERVER,			//Server notifies everyone a user joined

	USER_LEFT_SERVER,
	SERVER_CLOSING,

	MUTE_USER,
	UNMUTE_USER,
};

#pragma pack(push, 1)

struct Packet
{
	PacketEventId packetId;

	Packet(PacketEventId id) :
		packetId(id) 
	{}
};

//Base for all join events
struct JoinPacket : public Packet
{
	char username[sMAX_MESSAGE_LENGTH];

protected:
	JoinPacket(PacketEventId id, const std::string& name) :
		Packet(id)
	{
		strcpy(username, name.c_str());
	}
};

//Client sends to server to join.
struct RequestJoinServerPacket : public JoinPacket
{
	//Defaults to request join
	RequestJoinServerPacket(const std::string& name) :
		JoinPacket(PacketEventId::REQUEST_JOIN_SERVER, name)
	{}

protected:
	RequestJoinServerPacket(PacketEventId id, const std::string& name) :
		JoinPacket(id, name)
	{}
};

//Server sends to client that just joined.
//Server sends private welcome message to client.
struct JoinAcceptedPacket : public JoinPacket
{
	//The maximum amount of users that can be on the server.
	char maxUserCount;

	char connectedUserCount;

	//Holds user [id, username]
	char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH];	//[connectedUserCount][message length]

	JoinAcceptedPacket(const std::string& name, char maxUsers, char currentUserCount,
		char userInfo[][sMAX_USERNAME_LENGTH]) :
		JoinPacket(PacketEventId::JOIN_ACCEPTED, name),
		maxUserCount(maxUsers),
		connectedUserCount(currentUserCount)
	{
		//I tested this in another project and it works
		memcpy(connectedUserInfo, userInfo, sizeof(char) * sMAX_USERS * sMAX_USERNAME_LENGTH);
	}
};

//Server sends to everyone else in the chat that someone joined.
//Server sends a public message saying this user joined.
struct UserJoinedServerPacket : public RequestJoinServerPacket
{
	UserId userId;

	char connectedUserCount;

	//Holds user [id, username]
	char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH];

	UserJoinedServerPacket(UserId user, const std::string& name, char currentUserCount,
		char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH]) :
		RequestJoinServerPacket(PacketEventId::USER_JOINED_SERVER, name),
		userId(user),
		connectedUserCount(currentUserCount)
	{
		memcpy(connectedUserInfo, userInfo, sizeof(char)* sMAX_USERS * sMAX_USERNAME_LENGTH);
	}
};

//Client that left sends this to the server.
//The server sends this to everyone in the chat.
//The server then sends a public message saying this user left.
struct UserLeftServerPacket : public Packet
{
	UserId userId;

	UserLeftServerPacket(UserId user) :
		Packet(PacketEventId::USER_LEFT_SERVER),
		userId(user)
	{}
};

struct ServerClosingPacket : public Packet
{
	ServerClosingPacket() :
		Packet(PacketEventId::SERVER_CLOSING)
	{}
};

struct PublicMessagePacket : public Packet
{
	char message[512];

	UserId userId;

	//TODO: init user id also at some point
	PublicMessagePacket(const std::string& newMessage) : 
		Packet(PacketEventId::SEND_PUBLIC_MESSAGE)
	{ 
		strcpy(message, newMessage.c_str()); 
	}
}; 

#pragma pack(pop)

#endif