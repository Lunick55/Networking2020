#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <animal3D-A3DG/animal3D-A3DG.h>
#include <animal3D-A3DM/animal3D-A3DM.h>
//#include <A3_DEMO/a3_Networking/a3_Networking_gs_tictactoe.h>

typedef char UserId;

static const int INVALID_USER_ID = -1;
static const int sMAX_USERNAME_LENGTH = 20;
static const int sMAX_USERS = 64;
static const int sMAX_MESSAGE_LENGTH = 128;
static const int sDEFAULT_MAX_USERS = 10;
static const int sPORT = 5555;

static const int MAX_INPUT_LENGTH = 512;

//static const std::string PLAY_TICTACTOE = "tic";
//static const std::string PLAY_BATTLE = "battle";

extern struct a3_DemoState* gDemoState;
extern class TextFormatter& gTextFormatter;

struct AndrickUtils
{
	static a3vec3 createVec3(float x, float y, float z)
	{
		a3vec3 vec;
		vec.x = x;
		vec.y = y;
		vec.z = z;
		return vec;
	}
};

typedef a3vec4 Color;

struct AndrickColors
{
	//Because a3vec4s structs don't have constructors to initalize the vars...
	static Color createColor(float r, float g, float b, float a)
	{
		a3vec4 color;
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
		return color;
	}
};

static const Color WHITE = AndrickColors::createColor(1.0f, 1.0f, 1.0f, 1.0f);
static const Color BLACK = AndrickColors::createColor(0.0f, 0.0f, 0.0f, 1.0f);
static const Color DARK_GREY = AndrickColors::createColor(0.2f, 0.2f, 0.2f, 1.0f);
static const Color RED = AndrickColors::createColor(1.0f, 0.0f, 0.0f, 1.0f);
static const Color GREEN = AndrickColors::createColor(0.0f, 1.0f, 0.0f, 1.0f);
static const Color YELLOW = AndrickColors::createColor(0.0f, 1.0f, 1.0f, 1.0f);
static const Color LIGHT_BLUE = AndrickColors::createColor(0.0f, 0.5f, 2.0f, 1.0f);

enum class TextAlign : a3byte
{
	NONE,
	UNALIGN,
	LEFT,
	CENTER_X,
	CENTER_Y,
	CENTER,
	RIGHT
};

enum class CommandId : char
{
	INVALID_COMMAND = -1,
	LIST_PLAYERS,
	WHISPER,
};

enum class EventId : char
{
	INVALID_EVENT = -1,
	CHAT_MESSAGE
};

enum class GameType : unsigned char
{
	NONE,
	TICTAC,
	BATTLESHIP
};

const enum class MessageType : unsigned char
{
	GLOBAL,
	SPECTOR,
	PLAYER
};

const enum class SceneStateId : char
{
	INVALID_STATE = -1,
	EXIT_STATE,
	NEXT_SCENE_STATE
};

const enum class SceneId : char
{
	INVALID_SCENE = -1,
	EXIT,
	MAIN_MENU,
	LOBBY,
	//SelectRole,
	//Lobby,
	//Tictactoe,
	//Battleship,
	Count
};

enum class AuthorityId : unsigned char
{
	NORMAL,
	MODERATOR,
	ADMIN
};

enum PacketEventId : unsigned char
{
	SET_AUTHORITY = ID_USER_PACKET_ENUM,

	SEND_PUBLIC_MESSAGE_REQUEST,		//Clientside command
	SEND_PRIVATE_MESSAGE_REQUEST,		//Clientside command

	DELIVER_PUBLIC_MESSAGE,		//Serverside command
	DELIVER_PRIVATE_MESSAGE,	//Serverside command

	REQUEST_JOIN_SERVER,		//Client asks server to join
	JOIN_ACCEPTED,				//Server sends a welcome message to joined user
	USER_JOINED_SERVER,			//Server notifies everyone a user joined

	USER_LEFT_SERVER,
	SERVER_CLOSING,

	MUTE_USER,
	UNMUTE_USER,

	UPDATE_TICTAC_STATE,

	SERVER_TRAVEL,
	SETUP_TICTAC_GAME,
	UPDATE_BATTLE_STATE,
	SETUP_BATTLE_GAME,
	ASK_IF_BATTLE_HIT,
	REPLY_IF_BATTLE_HIT
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
	char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1];	//[connectedUserCount][message length]

	JoinAcceptedPacket(UserId user, const std::string& name, char maxUsers, char currentUserCount, char userInfo[][sMAX_USERNAME_LENGTH + 1]) : 
		userId(user),
		packetId(PacketEventId::JOIN_ACCEPTED), 
		maxUserCount(maxUsers),
		connectedUserCount(currentUserCount)
	{
		strcpy(username, name.c_str());
		memcpy(connectedUserInfo, userInfo, sizeof(char) * sMAX_USERS * (sMAX_USERNAME_LENGTH + 1));
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

	//char connectedUserCount;

	//Holds user [id, username]
	//char connectedUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1];

	UserJoinedServerPacket(UserId user, const std::string& name/*, char currentUserCount, char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1]*/) : 
		packetId(PacketEventId::USER_JOINED_SERVER),
		userId(user),
		username()
		//connectedUserCount(currentUserCount)
	{
		strcpy(username, name.c_str());
		//memcpy(connectedUserInfo, userInfo, sizeof(char) * sMAX_USERS * (sMAX_USERNAME_LENGTH + 1));
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

	UserLeftServerPacket(UserId user) : 
		packetId(PacketEventId::USER_LEFT_SERVER),
		userId(user)
	{

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

//The message sent to host asking to be delivered
#pragma pack(push, 1)
struct SendPublicMessageRequestPacket
{
	PacketEventId packetId;
	UserId userId;
	char message[sMAX_MESSAGE_LENGTH];
	char msgType;

	SendPublicMessageRequestPacket(UserId user, const std::string& newMessage, MessageType type = MessageType::GLOBAL) :
		packetId(PacketEventId::SEND_PUBLIC_MESSAGE_REQUEST),
		userId(user),
		msgType((char)type)
	{ 
		strcpy(message, newMessage.c_str()); 
	}
}; 
#pragma pack(pop)

#pragma pack(push, 1)
struct SendPrivateMessageRequestPacket
{
	PacketEventId packetId;
	UserId fromUserId;
	char toUsername[sMAX_MESSAGE_LENGTH];
	char message[sMAX_MESSAGE_LENGTH];

	SendPrivateMessageRequestPacket(UserId fromUser, const std::string& toUser, const std::string& newMessage) :
		packetId(PacketEventId::SEND_PRIVATE_MESSAGE_REQUEST),
		fromUserId(fromUser)
	{ 
		strcpy(message, newMessage.c_str()); 
		strcpy(toUsername, toUser.c_str()); 
	}
}; 
#pragma pack(pop)

//A message sent out. Broadcast by Host
#pragma pack(push, 1)
struct DeliverPublicMessagePacket
{
	PacketEventId packetId;
	UserId userId;
	char message[sMAX_MESSAGE_LENGTH];
	char msgType;

	//TODO: init user id also at some point
	DeliverPublicMessagePacket(UserId user, const std::string& newMessage, MessageType type = MessageType::GLOBAL) :
		packetId(PacketEventId::DELIVER_PUBLIC_MESSAGE),
		userId(user),
		msgType((char)type)
	{
		strcpy(message, newMessage.c_str());
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DeliverPrivateMessagePacket
{
	PacketEventId packetId;
	UserId fromUserId;
	UserId toUserId;
	char message[sMAX_MESSAGE_LENGTH];

	DeliverPrivateMessagePacket(UserId fromUser, UserId toUser, const std::string& newMessage) :
		packetId(PacketEventId::DELIVER_PRIVATE_MESSAGE),
		fromUserId(fromUser),
		toUserId(toUser)
	{ 
		strcpy(message, newMessage.c_str()); 
	}
}; 
#pragma pack(pop)

//#pragma pack(push, 1)
//struct UpdateTicTacState
//{
//	PacketEventId packetId;
//	gs_tictactoe_space_state tictactoeboard[3][3];
//	UserId fromUserId;
//
//	UpdateTicTacState(UserId fromUser, gs_tictactoe_space_state ticBoard[3][3]) :
//		packetId(PacketEventId::UPDATE_TICTAC_STATE),
//		fromUserId(fromUser)
//	{
//		memcpy(tictactoeboard, ticBoard, sizeof(gs_tictactoe_space_state) * 9);
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct SetupTictacGame
//{
//	PacketEventId packetId;
//	UserId player1Id;
//	char player1Username[sMAX_MESSAGE_LENGTH];
//	UserId player2Id;
//	char player2Username[sMAX_MESSAGE_LENGTH];
//
//	SetupTictacGame(UserId player1, const std::string& player1Name, UserId player2, const std::string& player2Name) :
//		packetId(PacketEventId::SETUP_TICTAC_GAME),
//		player1Id(player1),
//		player2Id(player2)
//	{
//		strcpy(player1Username, player1Name.c_str());
//		strcpy(player2Username, player2Name.c_str());
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct UpdateBattleState
//{
//	PacketEventId packetId;
//	char battleBoard[2][10][10];
//	UserId fromUserId;
//
//	UpdateBattleState(UserId fromUser, char batBoard[2][10][10]) :
//		packetId(PacketEventId::UPDATE_BATTLE_STATE),
//		fromUserId(fromUser)
//	{
//		memcpy(battleBoard, batBoard, sizeof(char) * (2 * 10 * 10));
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct SetupBattleGame
//{
//	PacketEventId packetId;
//	UserId player1Id;
//	char player1Username[sMAX_MESSAGE_LENGTH];
//	UserId player2Id;
//	char player2Username[sMAX_MESSAGE_LENGTH];
//
//	SetupBattleGame(UserId player1, const std::string& player1Name, UserId player2, const std::string& player2Name) :
//		packetId(PacketEventId::SETUP_BATTLE_GAME),
//		player1Id(player1),
//		player2Id(player2)
//	{
//		strcpy(player1Username, player1Name.c_str());
//		strcpy(player2Username, player2Name.c_str());
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct AskIfBattleHit
//{
//	PacketEventId packetId;
//	char location[2];
//	UserId fromUserId;
//
//	AskIfBattleHit(UserId fromUser, char loc[2]) :
//		packetId(PacketEventId::ASK_IF_BATTLE_HIT),
//		fromUserId(fromUser)
//	{
//		memcpy(location, loc, sizeof(char) * (2));
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ReplyIfBattleHit
//{
//	PacketEventId packetId;
//	char location[2];
//	char hitInfo;
//	UserId fromUserId;
//
//	ReplyIfBattleHit(UserId fromUser, char loc[2], char hitInf) :
//		packetId(PacketEventId::REPLY_IF_BATTLE_HIT),
//		fromUserId(fromUser),
//		hitInfo(hitInf)
//	{
//		memcpy(location, loc, sizeof(char) * (2));
//	}
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ServerTravel
//{
//	PacketEventId packetId;
//	SceneId sceneId;
//
//	ServerTravel(SceneId scene) :
//		packetId(PacketEventId::SERVER_TRAVEL),
//		sceneId(scene)
//	{
//
//	}
//};
//#pragma pack(pop)

#endif