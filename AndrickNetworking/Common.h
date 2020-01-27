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


//Client sends to server to join.
#pragma pack(push, 1)
struct RequestJoinServerPacket
{
	PacketEventId packetId;
	char username[sMAX_USERNAME_LENGTH];

	RequestJoinServerPacket(const std::string& name) : 
		packetId(PacketEventId::REQUEST_JOIN_SERVER) 
	{ 
		strcpy(username, name.c_str()); 
	}
};
#pragma pack(pop)

//Server sends to client that just joined.
//Server sends private welcome message to client.
#pragma pack(push, 1)
struct JoinAcceptedPacket
{
	PacketEventId packetId;
	char username[sMAX_USERNAME_LENGTH];

	//The maximum amount of users that can be on the server.
	char maxUserCount;
	char connectedUserCount;

	//The server assigns the client a userId and sends it to them!
	UserId userId;

	//Holds user [id, username]
	char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH];	//[connectedUserCount][message length]

	JoinAcceptedPacket(UserId user, const std::string& name, char maxUsers, char currentUserCount, char userInfo[][sMAX_USERNAME_LENGTH]) : 
		userId(user),
		packetId(PacketEventId::JOIN_ACCEPTED), 
		maxUserCount(maxUsers),
		connectedUserCount(currentUserCount)
	{
		strcpy(username, name.c_str());
		memcpy(connectedUserInfo, userInfo, sizeof(char) * sMAX_USERS * sMAX_USERNAME_LENGTH);
	}
};
#pragma pack(pop)

//Server sends to everyone else in the chat that someone joined.
//Server sends a public message saying this user joined.
#pragma pack(push, 1)
struct UserJoinedServerPacket
{
	PacketEventId packetId;
	UserId userId;
	char username[sMAX_USERNAME_LENGTH];
	char connectedUserCount;

	//Holds user [id, username]
	char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH];

	UserJoinedServerPacket(UserId user, const std::string& name, char currentUserCount, char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH]) : 
		packetId(PacketEventId::USER_JOINED_SERVER),
		userId(user), 
		connectedUserCount(currentUserCount)
	{
		strcpy(username, name.c_str());
		memcpy(connectedUserInfo, userInfo, sizeof(char) * sMAX_USERS * sMAX_USERNAME_LENGTH);
	}
};
#pragma pack(pop)

//Client that left sends this to the server.
//The server sends this to everyone in the chat.
//The server then sends a public message saying this user left.
#pragma pack(push, 1)
struct UserLeftServerPacket
{
	PacketEventId packetId;
	UserId userId;
	char username[sMAX_USERNAME_LENGTH];

	UserLeftServerPacket(UserId user, const std::string& name) : 
		packetId(PacketEventId::USER_LEFT_SERVER),
		userId(user)
	{
		strcpy(username, name.c_str());
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ServerClosingPacket
{
	PacketEventId packetId;

	ServerClosingPacket() :
		packetId(PacketEventId::SERVER_CLOSING)
	{
		
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PublicMessagePacket
{
	PacketEventId packetId;
	UserId userId;
	char username[sMAX_USERNAME_LENGTH];
	char message[sMAX_MESSAGE_LENGTH];

	//TODO: init user id also at some point
	PublicMessagePacket(UserId user, const std::string& name, const std::string& newMessage) :
		packetId(PacketEventId::SEND_PUBLIC_MESSAGE),
		userId(user)
	{ 
		strcpy(username, name.c_str()); 
		strcpy(message, newMessage.c_str()); 
	}
}; 
#pragma pack(pop)

#endif