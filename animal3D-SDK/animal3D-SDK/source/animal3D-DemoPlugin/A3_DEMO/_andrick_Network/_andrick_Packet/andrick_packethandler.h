#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

class PacketHandler
{
	friend class Server;
public:
	PacketHandler(bool isServer);
	virtual ~PacketHandler() = default;

	inline bool isInitialized() const { return mpPeer != nullptr; }
	inline bool isServer() const { return mIsServer; }

	bool startup(int maxConnections);
	bool shutdown();
	bool connect(const char* ipAddress);
	bool disconnect();
	int processInboundPackets();
	bool broadcast(const char* packetData, std::size_t packetSize) const;
	bool sendToOne(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress) const;

	void setServerAddress(const RakNet::SystemAddress& address) { mServerAddress = address; }
	const RakNet::SystemAddress& getServerAddress() const { return mServerAddress; }

	const std::size_t getMaxConnections() const { return mMaxConnections; }

private:
	bool mIsServer;
	std::size_t mMaxConnections;
	RakNet::SystemAddress mServerAddress;

	RakNet::RakPeerInterface* mpPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
};

#endif