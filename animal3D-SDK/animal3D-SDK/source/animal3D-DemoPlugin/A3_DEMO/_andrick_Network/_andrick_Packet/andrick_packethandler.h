#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

class PacketHandler
{
public:
	PacketHandler(bool isServer);
	virtual ~PacketHandler() = default;

	inline bool isInitialized() const { return mpPeer != nullptr; };

	bool startup(int maxConnections);
	bool shutdown();
	bool connect(const char* ipAddress);
	bool disconnect();
	int processInboundPackets();
	bool broadcast(const char* packetData, std::size_t packetSize) const;
	bool sendToOne(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress) const;

	const RakNet::SystemAddress& getServerAddress() const { return mServerAddress; };

private:
	bool mIsServer;
	int mMaxConnections;
	RakNet::SystemAddress mServerAddress;

	RakNet::Packet* mpPacket;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
};

#endif