#ifndef CHAT_ROOM_CLIENT_H_
#define CHAT_ROOM_CLIENT_H_

#include "Common.h"
#include "IPacketReceiver.h"
#include "IPacketSender.h"
#include <map>

class User;

class ChatRoomClient : public IPacketReceiver, IPacketSender
{
	friend class ChatRoomScene;

public:
	static bool isInitialized();
	static bool initChatRoom(const std::string& serverIP, const std::string& clientUsername);

	ChatRoomClient(const std::string& serverIP, const std::string& username);

	void update();

	bool connectToServer();
	void leaveServer();

private:
	static std::shared_ptr<ChatRoomClient> spInstance;
	
	std::string mServerIP;
	std::string mUsername;

	//Gets initialized when the server sends back a packet with
	//the info.
	std::unique_ptr<User> mClient;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::Packet* mpPacket;
	RakNet::SocketDescriptor mSocketDescriptor;

	virtual void receivePacket() override;
	virtual void sendPacket(const Packet& packet) override;

	void requestToJoinServer();
};

#endif