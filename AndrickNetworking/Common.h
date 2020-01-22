#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <memory>
#include <string>

#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>

typedef unsigned char UserId;

static const int sMAX_USERNAME_LENGTH = 20;
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

enum class PacketEventId : unsigned char
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
	UNMUTE_USER
};

struct Packet
{
	PacketEventId packetId;

	Packet(PacketEventId id) :
		packetId(id) 
	{}
};

#endif