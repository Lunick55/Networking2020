#include "ChatRoomServer.h"
#include "Command.h"
#include "User.h"

std::shared_ptr<ChatRoomServer> ChatRoomServer::spInstance = nullptr;

bool ChatRoomServer::isInitialized()
{
	return (spInstance != nullptr);
}

bool ChatRoomServer::initChatRoom(const std::string& address, const int port, const int maxUsers)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<ChatRoomServer>(address, port, maxUsers);
	}

	return isInitialized();
}

const ChatRoomServer& ChatRoomServer::get()
{
	assert(isInitialized());
	return *spInstance;
}

ChatRoomServer::ChatRoomServer(const std::string& address, const int port, const int maxUsers) :
	mIP_ADDRESS(address), mPORT(port), mMAX_USERS(maxUsers), mIsRunning(false),
	mpPeer(RakNet::RakPeerInterface::GetInstance())
{

}

void ChatRoomServer::startChatRoom()
{
	RakNet::RakPeerInterface* pPeer = RakNet::RakPeerInterface::GetInstance();

	if (mIsRunning)
	{
		std::cerr << "Server is already running." << std::endl;
		return;
	}
	else
	{
		RakNet::SocketDescriptor sd(mPORT, 0);
		pPeer->Startup(mMAX_USERS, &sd, 1);

		std::cout << "Server is running!" << std::endl;

		mIsRunning = true;
	}
}

void ChatRoomServer::closeChatRoom()
{
	std::unique_ptr<Packet> serverClosing = std::make_unique<ServerClosingPacket>();

	auto iter = mpConnectedUsers.begin();
	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		sendPacket(*serverClosing.get());
	}

	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
}

void ChatRoomServer::sendPacket(const Packet& packet)
{
	std::map<UserId, std::unique_ptr<User>>::iterator iter;

	switch (packet.packetId)
	{
	case PacketEventId::SET_AUTHORITY:
		break;

	case PacketEventId::SEND_PUBLIC_MESSAGE:
		sendPublicMessage(packet);
		break;

	case PacketEventId::SEND_PRIVATE_MESSAGE:
		sendPrivateMessage(packet);
		break;

	case PacketEventId::DELIVER_PUBLIC_MESSAGE:
		break;

	case PacketEventId::DELIVER_PRIVATE_MESSAGE:
		break;

	case PacketEventId::REQUEST_JOIN_SERVER:
		break;

	case PacketEventId::JOIN_ACCEPTED:
		break;

	case PacketEventId::USER_JOINED_SERVER:
		break;

	case PacketEventId::USER_LEFT_SERVER:
		break;

	case PacketEventId::SERVER_CLOSING:
		break;

	case PacketEventId::MUTE_USER:
		break;

	case PacketEventId::UNMUTE_USER:
		break;

	default:
		std::cerr << "Unknown packet id" << std::endl;
	}
}

//Check if a packet is incoming
void ChatRoomServer::receivePacket()
{

}

void ChatRoomServer::sendPublicMessage(const Packet& packet)
{
	mpPeer->Send((const char*)(&packet), sizeof(packet),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ChatRoomServer::sendPrivateMessage(const Packet& packet)
{
	const PrivateMessageCommandPacket& p = static_cast<const PrivateMessageCommandPacket&>(packet);

	//Send to host
	mpPeer->Send((const char*)(&packet), sizeof(packet),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mpHost.get()->getAddress(), false);

	//Send to target user
	auto iter = mpConnectedUsers.find(p.userToSendTo);
	if (iter != mpConnectedUsers.end())
	{
		const User& targetUser = *iter->second;

		mpPeer->Send((const char*)(&packet), sizeof(packet),
			PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
			0, targetUser.getAddress(), false);
	}

	//Send to original user
	//If original user is the host, we don't need to send it to them again
	if (p.userWhoExecutedCommand == mpHost.get()->getUserId()) return;

	//We shouldn't need to check if they're in the list
	//since this user just sent this command, but we can always 
	//perform a check like above in the future if necessary
	const User& targetUser = *mpConnectedUsers.find(p.userWhoExecutedCommand)->second;

	mpPeer->Send((const char*)(&packet), sizeof(packet),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, targetUser.getAddress(), false);
}