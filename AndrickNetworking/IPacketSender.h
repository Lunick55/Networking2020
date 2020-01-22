#ifndef IPACKET_SENDER_H_
#define IPACKET_SENDER_H_

#include "Common.h"

struct IPacketSender
{
	virtual void sendPacket(const Packet& packet) = 0;
};

#endif