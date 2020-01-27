#ifndef CHAT_ROOM_SERVER_H_
#define CHAT_ROOM_SERVER_H_

#include <map>
#include <list>

#include "IPacketSender.h"
#include "IPacketReceiver.h"
#include "Command.h"

class User;

/*
* This class gets created if the user decides to host a server.
*/
class ChatRoomServer : public IPacketSender, IPacketReceiver
{
	friend class ChatRoomScene;

public:
	static bool isInitialized();
	static bool initChatRoom(const int port, const int maxUsers, const std::string& hostUsername);

	explicit ChatRoomServer(const int port, const int maxUsers, const std::string& hostUsername);
	virtual ~ChatRoomServer() = default;

	void update();

	//Start the server
	bool startChatRoom();
	void closeChatRoom();

private:
	static std::shared_ptr<ChatRoomServer> spInstance;

	static UserId sNextUniqueId;

	const int mMAX_USERS;
	const int mPORT;
	std::shared_ptr<User> mpHost;
	char mServerUserInfo[sMAX_USERS][sMAX_USERNAME_LENGTH];

	std::list<std::string> messageLog;

	bool mIsRunning;
	RakNet::RakPeerInterface* mpPeer;
	RakNet::Packet* mpPacket;
	RakNet::SocketDescriptor mSocketDescriptor;

	std::map<UserId, std::shared_ptr<User>> mpConnectedUsers;
	std::map<PacketEventId, std::shared_ptr<Command>> mpValidCommands;

	void sendPublicMessage(std::shared_ptr<User> user, std::string message);//Packet& packet);
	void sendPrivateMessage();

	void updateServerUserInfo();
	std::shared_ptr<User> addNewUser(const RequestJoinServerPacket& requestPacket, RakNet::SystemAddress ipAddress);
	void removeUser(UserId userId);

	//virtual void sendPacket(const Packet& packet) override;
	virtual void receivePacket() override;
};

#endif