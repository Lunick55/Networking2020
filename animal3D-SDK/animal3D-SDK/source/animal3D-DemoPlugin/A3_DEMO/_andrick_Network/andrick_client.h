#ifndef CHAT_ROOM_CLIENT_H_
#define CHAT_ROOM_CLIENT_H_

#include "andrick_common.h"
#include "andrick_user.h"
#include <map>

/*
* This class gets created if the user decides to join another server.
*/
class Client : public User
{
	friend class ChatRoomScene;

public:
	static bool isInitialized();
	static bool initChatRoom(bool isHost, const std::string& serverIP, const std::string& clientUsername);
	static bool isHost() { return spInstance->mIsHost; };

	Client(bool isHost, const std::string& serverIP, const std::string& username);

	void update();

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

	virtual void receivePacket() override;

	void sendPublicMessage(const std::string& message);
	void sendPrivateMessageRequest(const std::string& message, const std::string& toUsername);

	void addUserIdToMap(UserId userId, char name[sMAX_USERNAME_LENGTH]);
	void removeUserFromMap(UserId userId);
	void initUsernameMap(char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1], int connectedUsers);

	void requestToJoinServer();
};

#endif