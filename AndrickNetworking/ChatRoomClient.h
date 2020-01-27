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
	static bool initChatRoom(bool isHost, const std::string& serverIP, const std::string& clientUsername);
	static bool isHost() { return spInstance->mIsHost; };

	ChatRoomClient(bool isHost, const std::string& serverIP, const std::string& username);

	void update();

	bool connectToServer();
	void leaveServer();

private:
	static std::shared_ptr<ChatRoomClient> spInstance;
	
	std::string mServerIP;
	std::string mUsername;
	bool mIsHost;

	//Gets initialized when the server sends back a packet with the client.
	std::unique_ptr<User> mpClient;

	//Gets initialized and updated when users join and leave the server.
	std::map<UserId, std::string> mUsernameMap;

	RakNet::SystemAddress mHostAddress;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::Packet* mpPacket;
	RakNet::SocketDescriptor mSocketDescriptor;

	virtual void receivePacket() override;
	//virtual void sendPacket(const Packet& packet) override;

	void sendPublicMessage(const std::string& message);
	bool sendPrivateMessageRequest(const std::string& message, const std::string& toUsername);

	void addUserIdToMap(UserId userId, char name[sMAX_USERNAME_LENGTH]);
	void removeUserFromMap(UserId userId);
	void initUsernameMap(char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1], int connectedUsers);

	void requestToJoinServer();
};

#endif