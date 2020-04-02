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

typedef char UserId;
typedef unsigned int UnitID;

static const int INVALID_USER_ID = -1;
static const int sMAX_USERNAME_LENGTH = 20;
static const int sMAX_USERS = 64;
static const int sMAX_MESSAGE_LENGTH = 128;
static const int sDEFAULT_MAX_USERS = 10;
static const int sPORT = 12975;

static const int MAX_INPUT_LENGTH = 512;

static constexpr int BOID_COUNT = 1;

extern struct a3_DemoState* gDemoState;
extern class TextFormatter& gTextFormatter;
extern class EventSystem& gEventSystem;

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
	NEW_INCOMING_CONNECTION,///Client -> Server - see if we can get inital connection
	CONNECTION_REQUEST_ACCEPTED,///Server -> Client - server got the connection and you can join
	CONNECTION_REQUEST_FAILED,///Server -> Client - server got the connection but it's full so bye
	CONNECTION_REQUEST_JOIN,///Client -> Server - client asks if it can join with andrick specific data (username, authority, etc)
	CONNECTION_JOIN_ACCEPTED,///Server -> Client - Everything is good, you can enter the server
	CONNECTION_JOIN_FAILED,///Server -> Client - Disconnect a user
	CONNECTION_NEW_USER_JOINED,///Server -> All Clients - A new user joined the server
	USER_DISCONNECTED,///Disconnected Client -> Server. Server -> All Clients - A client left the server
	CHAT_MESSAGE,
	INCREMENT_THE_SPACE,
	//GENERIC_EVENT,
	BOID_DATA_EVENT,
	COMMAND
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
	SERVER_BOIDS,
	CLIENT_BOIDS,
	LOBBY,
	MINIGAME,
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
	andrick_ID_SET_AUTHORITY = ID_USER_PACKET_ENUM,

	andrick_ID_SEND_PUBLIC_MESSAGE_REQUEST,		//Clientside command
	andrick_ID_SEND_PRIVATE_MESSAGE_REQUEST,	//Clientside command

	andrick_ID_DELIVER_PUBLIC_MESSAGE,			//Serverside command
	andrick_ID_DELIVER_PRIVATE_MESSAGE,			//Serverside command

	andrick_ID_CONNECTION_REQUEST_ACCEPTED,
	andrick_ID_CONNECTION_ATTEMPT_FAILED,
	andrick_ID_REQUEST_JOIN_SERVER,				//Client asks server to join
	andrick_ID_JOIN_ACCEPTED,					//Server sends a welcome message to joined user
	andrick_ID_JOIN_FAILED,						//Server sends this to client, client disconnects
	andrick_ID_USER_JOINED_SERVER,				//Server notifies everyone a user joined

	andrick_ID_USER_DISCONNECTED,
	andrick_ID_SERVER_CLOSING,

	////////////////////////

	andrick_ID_MUTE_USER,
	andrick_ID_UNMUTE_USER,

	andrick_ID_WHISPER_COMMAND,


	//////////////////////////
	// BOID EVENT TYPES
	//////////////////////////

	andrick_ID_BOID_DATA_PUSH_EVENT,
	andrick_ID_BOID_DATA_SHARE_EVENT,
	andrick_ID_BOID_DATA_COUPLE_EVENT,

	//////////////////////////

	andrick_ID_SERVER_TRAVEL,
	andrick_ID_UPDATE_TICTAC_STATE,
	andrick_ID_SETUP_TICTAC_GAME,
	andrick_ID_UPDATE_BATTLE_STATE,
	andrick_ID_SETUP_BATTLE_GAME,
	andrick_ID_ASK_IF_BATTLE_HIT,
	andrick_ID_REPLY_IF_BATTLE_HIT
};

#ifdef __cplusplus
extern "C"
{
#endif
	// C header here
	extern void initializeServer();
	extern void initializeClient();
	extern void shutdownRakNet();
	extern RakNet::Time getTime();
#ifdef __cplusplus
}
#endif

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