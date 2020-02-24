#ifndef CONNECTABLE_H_
#define CONNECTABLE_H_

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

class IConnectable
{
public:
	IConnectable();
	virtual ~IConnectable();

protected:
	RakNet::Packet* mpPacket;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::SystemAddress mAddress;
	RakNet::SocketDescriptor mSocketDescriptor;
};

#endif