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
static const int sPORT = 5555;

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