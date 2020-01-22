#ifndef IPACKET_RECEIVER_H_
#define IPACKET_RECEIVER_H_

#include "Common.h"

class IPacketReceiver
{
	virtual void receivePacket() = 0;
};

#endif