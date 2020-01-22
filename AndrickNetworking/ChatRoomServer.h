#ifndef CHAT_ROOM_SERVER_H_
#define CHAT_ROOM_SERVER_H_

#include <map>

#include "IPacketSender.h"
#include "IPacketReceiver.h"
#include "Command.h"

class User;

/*
* This class gets created if the user decides to host a server.
*/
class ChatRoomServer : private IPacketSender, IPacketReceiver
{
public:
	static bool isInitialized();
	static bool initChatRoom(const std::string& address, const int port, const int maxUsers);
	static const ChatRoomServer& get();

	explicit ChatRoomServer(const std::string& address, const int port, const int maxUsers);
	virtual ~ChatRoomServer() = default;

	//Start the server
	void startChatRoom();
	void closeChatRoom();

private:
	static std::shared_ptr<ChatRoomServer> spInstance;

	static UserId nextUniqueId;

	const int mMAX_USERS;
	const int mPORT;
	const std::string mIP_ADDRESS;
	std::unique_ptr<User> mpHost;

	bool mIsRunning;
	RakNet::RakPeerInterface* mpPeer;

	std::map<UserId, std::unique_ptr<User>> mpConnectedUsers;
	std::map<PacketEventId, std::unique_ptr<Command>> mpValidCommands;

	void sendPublicMessage(const Packet& packet);
	void sendPrivateMessage(const Packet& packet);

	virtual void sendPacket(const Packet& packet) override;
	virtual void receivePacket() override;
};

#pragma pack(push, 1)

//Base for all join events
struct JoinPacket : public Packet
{
	char username[sMAX_MESSAGE_LENGTH];

protected:
	JoinPacket(PacketEventId id, const std::string& name) :
		Packet(id)
	{
		strcpy(username, name.c_str());
	}

};

//Client sends to server to join.
struct RequestJoinServerPacket : public JoinPacket
{
	//Defaults to request join
	RequestJoinServerPacket(const std::string& name) :
		JoinPacket(PacketEventId::REQUEST_JOIN_SERVER, name)
	{}

protected:
	RequestJoinServerPacket(PacketEventId id, const std::string& name) :
		JoinPacket(id, name)
	{}
};

//Server sends to client that just joined.
//Server sends private welcome message to client.
struct JoinAcceptedPacket : public JoinPacket
{
	//The maximum amount of users that can be on the server.
	char maxUserCount;

	char connectedUserCount;

	//Holds user [id, username]
	char connectedUsers[][sMAX_USERNAME_LENGTH];	//[connectedUserCount][message length]

	JoinAcceptedPacket(const std::string& name, char maxUsers, char currentUserCount,
		char userInfo[][sMAX_USERNAME_LENGTH]) :
		JoinPacket(PacketEventId::JOIN_ACCEPTED, name),
		maxUserCount(maxUsers),
		connectedUserCount(currentUserCount)
	{
		//I tested this in another project and it works
		memcpy(connectedUsers, userInfo, sizeof(char) * currentUserCount * sMAX_USERNAME_LENGTH);
	}
};

//Server sends to everyone else in the chat that someone joined.
//Server sends a public message saying this user joined.
struct UserJoinedServerPacket : public RequestJoinServerPacket
{
	UserId userId;

	UserJoinedServerPacket(UserId user, const std::string& name) :
		RequestJoinServerPacket(PacketEventId::USER_JOINED_SERVER, name),
		userId(user)
	{}
};

//Client that left sends this to the server.
//The server sends this to everyone in the chat.
//The server then sends a public message saying this user left.
struct UserLeftServerPacket : public Packet
{
	UserId userId;

	UserLeftServerPacket(UserId user) :
		Packet(PacketEventId::USER_LEFT_SERVER),
		userId(user)
	{}
};

struct ServerClosingPacket : public Packet
{
	ServerClosingPacket() :
		Packet(PacketEventId::SERVER_CLOSING)
	{}
};

#pragma pack(pop)

#endif