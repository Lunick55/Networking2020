#ifndef CHAT_ROOM_CLIENT_H_
#define CHAT_ROOM_CLIENT_H_

#include "../_andrick_Utils/andrick_common.h"
#include "andrick_user.h"
#include <map>

struct a3_DemoState;

/*
* This class gets created if the user decides to join another server.
*/
class Client
{
	friend class LobbyScene;
	friend class TictactoeScene;

public:
	static bool isInitialized();
	static bool initChatRoom(bool isHost, const std::string& serverIP, const std::string& clientUsername);
	static bool isHost() { return spInstance->mIsHost; };

	Client(bool isHost, const std::string& serverIP, const std::string& username);

	void update(const a3_DemoState* demoState);

	bool connectToServer();
	void leaveServer();

private:
	static std::shared_ptr<Client> spInstance;
	
	std::string mServerIP;
	std::string mUsername;
	int mMaxUsers;
	bool mIsHost;

	//Gets initialized when the server sends back a packet with the client.
	std::unique_ptr<User> mpClient;

	//Gets initialized and updated when users join and leave the server.
	std::map<UserId, std::string> mUsernameMap;

	RakNet::SystemAddress mHostAddress;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::Packet* mpPacket;
	RakNet::SocketDescriptor mSocketDescriptor;

	void receivePacket(const a3_DemoState* demoState);

	void sendPublicMessage(const std::string& message);
	void sendPrivateMessageRequest(const std::string& message, const std::string& toUsername);

	void addUserIdToMap(UserId userId, char name[sMAX_USERNAME_LENGTH]);
	void removeUserFromMap(UserId userId);
	void initUsernameMap(char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1], int connectedUsers);

	void requestToJoinServer(const a3_DemoState* demoState);
};

#endif