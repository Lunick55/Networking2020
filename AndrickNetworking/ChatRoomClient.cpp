#include "ChatRoomClient.h"
#include "User.h"
#include "SceneManager.h"

std::shared_ptr<ChatRoomClient> ChatRoomClient::spInstance = nullptr;

bool ChatRoomClient::isInitialized()
{
	return (spInstance != nullptr);
}

bool ChatRoomClient::initChatRoom(const std::string& serverIP, const std::string& hostUsername)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<ChatRoomClient>(serverIP, hostUsername);
	}

	return isInitialized();
}

ChatRoomClient::ChatRoomClient(const std::string& serverIP, const std::string& username) :
	mServerIP(serverIP),
	mUsername(username),
	mClient(nullptr),
	mpPeer(RakNet::RakPeerInterface::GetInstance()),
	mpPacket(nullptr)
{
	mpPeer->Startup(1, &mSocketDescriptor, 1);
}

void ChatRoomClient::update()
{
	receivePacket();
}

//Always looping to get new packet data
void ChatRoomClient::receivePacket()
{
	//Incoming packets to client from server.
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Our connection request has been accepted." << std::endl;
			requestToJoinServer();
			break;

		case PacketEventId::SET_AUTHORITY:
			break;

		case PacketEventId::SEND_PUBLIC_MESSAGE:
			//TODO:implement this
			std::cout << "something exists" << std::endl;
			break;

		case PacketEventId::SEND_PRIVATE_MESSAGE:
			break;

		//why would the client deliver a message?
		//case PacketEventId::DELIVER_PUBLIC_MESSAGE:
		//	break;
		//
		//case PacketEventId::DELIVER_PRIVATE_MESSAGE:
		//	break;

		case PacketEventId::REQUEST_JOIN_SERVER:
			break;

		case PacketEventId::JOIN_ACCEPTED:
		{
			Packet* data = (JoinAcceptedPacket*)mpPacket->data;
			
			//Make sure there was no corruption of data over the network.
			assert(mpPacket->length == sizeof(JoinAcceptedPacket));
			if (mpPacket->length != sizeof(JoinAcceptedPacket))
			{
				std::cout << "Uh oh! Invalid length for AndrickPacket!" << std::endl;
				break;
			}
			else
			{
				std::cout << "We got the packet!" << std::endl;
			}

			break;
		}
		//Are any of the next 3 needed?
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
			printf("Message with identifier %i has arrived.\n", mpPacket->data[0]);
			break;
		}
	}
}

//void ChatRoomClient::sendPacket(const Packet& packet)
//{
//	const Packet p = packet;
//
//	switch (packet.packetId)
//	{
//	case PacketEventId::SET_AUTHORITY:
//		break;
//
//	case PacketEventId::SEND_PUBLIC_MESSAGE:
//		break;
//
//	case PacketEventId::SEND_PRIVATE_MESSAGE:
//		break;
//
//	case PacketEventId::DELIVER_PUBLIC_MESSAGE:
//		break;
//
//	case PacketEventId::DELIVER_PRIVATE_MESSAGE:
//		break;
//
//	case PacketEventId::REQUEST_JOIN_SERVER:
//		mpPeer->Send(
//			(const char*)(&p),
//			sizeof(RequestJoinServerPacket),
//			PacketPriority::IMMEDIATE_PRIORITY,
//			PacketReliability::RELIABLE_ORDERED,
//			0,
//			mpPacket->systemAddress,
//			false
//		);
//
//		break;
//
//	case PacketEventId::JOIN_ACCEPTED:
//		break;
//
//	case PacketEventId::USER_JOINED_SERVER:
//		break;
//
//	case PacketEventId::USER_LEFT_SERVER:
//		break;
//
//	case PacketEventId::SERVER_CLOSING:
//		break;
//
//	case PacketEventId::MUTE_USER:
//		break;
//
//	case PacketEventId::UNMUTE_USER:
//		break;
//
//	default:
//		std::cerr << "Unknown packet id" << std::endl;
//	}
//}

bool ChatRoomClient::connectToServer()
{
	RakNet::ConnectionAttemptResult result = mpPeer->Connect(mServerIP.c_str(), sPORT, 0, 0);
	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

void ChatRoomClient::leaveServer()
{
	//Send a leaving packet.
	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
	spInstance = nullptr;
	SceneManager::switchScene(SceneId::MAIN_MENU);
}

void ChatRoomClient::requestToJoinServer()
{
	//We are a client connecting.
	std::unique_ptr<Packet> requestJoinPacket = std::make_unique<RequestJoinServerPacket>(
		ChatRoomClient::spInstance->mUsername
		);

	//ChatRoomClient::spInstance->sendPacket(*requestJoinPacket);
	mpPeer->Send((const char*)(&requestJoinPacket), sizeof(RequestJoinServerPacket), PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
}