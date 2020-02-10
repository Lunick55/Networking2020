#ifndef CHAT_ROOM_SERVER_H_
#define CHAT_ROOM_SERVER_H_

#include <map>
#include <list>

#include "../_andrick_Utils/andrick_common.h"
#include "andrick_client.h"
#include "andrick_user.h"

struct a3_DemoState;

/*
* This class gets created if the user decides to host a server.
*/
class Host
{
	friend class LobbyScene;
	friend class TictactoeScene;
	friend class BattleShipScene;

public:
	static bool isInitialized();
	static bool initChatRoom(const int port, const int maxUsers, const std::string& hostUsername);

	explicit Host(const int port, const int maxUsers, const std::string& hostUsername);
	virtual ~Host() = default;

	static void listUserInfoRequest(const a3_DemoState* demoState);
	void printUserInfo(const a3_DemoState* demoState);

	void update(const a3_DemoState* demoState);

	std::shared_ptr<User> getUserFromId(UserId userId);

	//Start the server
	bool startChatRoom(const a3_DemoState* demoState);
	void closeChatRoom();

private:
	static std::shared_ptr<Host> spInstance;

	static UserId sNextUniqueId;

	const int mMAX_USERS;
	const int mPORT;
	std::shared_ptr<User> mpHost;
	char mServerUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1];

	std::list<std::string> messageLog;

	bool mIsRunning;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::Packet* mpPacket;
	RakNet::SocketDescriptor mSocketDescriptor;

	std::map<UserId, std::shared_ptr<User>> mpConnectedUsers;
	//std::map<PacketEventId, std::shared_ptr<Command>> mpValidCommands;

	void deliverPersonalMessage(const a3_DemoState* demoState, const std::string& userName, const std::string& message);

	void deliverPublicMessage(const a3_DemoState* demoState, std::shared_ptr<User> user, std::string message);
	void deliverPrivateMessage(const a3_DemoState* demoState, UserId fromUserId, UserId toUserId, const std::string& message);

	void updateServerUserInfo();
	std::shared_ptr<User> addNewUser(const RequestJoinServerPacket& requestPacket, RakNet::SystemAddress ipAddress);
	void removeUser(const a3_DemoState* demoState, UserId userId);

	void receivePacket(const a3_DemoState* demoState);

	void broadcastPacket(const char* packetData, std::size_t packetSize);
	void sendOncePacket(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress);
};

#endif
