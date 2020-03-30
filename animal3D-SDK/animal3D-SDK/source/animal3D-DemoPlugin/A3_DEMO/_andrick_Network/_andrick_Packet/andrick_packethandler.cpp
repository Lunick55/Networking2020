#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packet.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

PacketHandler::PacketHandler(bool isServer) :
	mIsServer(isServer),
	mMaxConnections(0),
	mpPeer(nullptr)
{

}

bool PacketHandler::startup(int maxConnections)
{
	if (!mpPeer)
	{
		mpPeer = RakNet::RakPeerInterface::GetInstance();

		if (mpPeer)
		{
			if (mIsServer)
			{
				mSocketDescriptor = RakNet::SocketDescriptor(sPORT, 0);
			}

			if (mpPeer->Startup(maxConnections, &mSocketDescriptor, 1) == RakNet::StartupResult::RAKNET_STARTED)
			{
				mpPeer->SetMaximumIncomingConnections(maxConnections);
				mpPeer->SetOccasionalPing(true);
				mMaxConnections = maxConnections;

				if (mIsServer)
				{
					mServerAddress = mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID());
				}
				else
				{
					gDemoState->mpClient->setAddress(mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID()));
				}

				return true;
			}
		}
	}

	return false;
}

bool PacketHandler::shutdown()
{
	if (mpPeer)
	{
		mpPeer->Shutdown(500);
		RakNet::RakPeerInterface::DestroyInstance(mpPeer);
		mpPeer = nullptr;

		gDemoState->mIsOnline = false;
		return true;
	}

	return false;
}

bool PacketHandler::connect(const char* ipAddress)
{
	if (mpPeer)
	{
		RakNet::ConnectionAttemptResult result = mpPeer->Connect(ipAddress, sPORT, 0, 0);
		return (result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED);
	}

	return false;
}

bool PacketHandler::disconnect()
{
	if (mpPeer)
	{
		a3ui16 i, j = mpPeer->NumberOfConnections();

		for (i = 0; i < j; ++i)
		{
			mpPeer->CloseConnection(mpPeer->GetSystemAddressFromIndex(i), true);
		}

		gDemoState->mIsOnline = false;
		return true;
	}

	return false;
}

int PacketHandler::processInboundPackets()
{
	RakNet::Packet* packet;
	//RakNet::MessageID messageId;
	int packetsProcessed = 0;

	for (packet = mpPeer->Receive(); 
		packet != nullptr; 
		mpPeer->DeallocatePacket(packet), packet = mpPeer->Receive(), ++packetsProcessed)
	{
		//RakNet::BitStream inBitStream(packet->data, packet->length, false);
		//inBitStream.Read(messageId);

		std::vector<std::shared_ptr<Event>> newEvents;

		switch (packet->data[0])
		{
			////////////////////////////////////////////////
			// RAKNET PACKETS                             
			////////////////////////////////////////////////
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << std::endl;
			break;
		}
		case ID_NEW_INCOMING_CONNECTION:
		{
			std::cout << "ID_NEW_INCOMING_CONNECTION" << std::endl;
			newEvents.push_back(std::make_shared<NewIncomingConnectionEvent>(packet->systemAddress));
			break;
		}
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "ID_NO_FREE_INCOMING_CONNECTIONS" << std::endl;
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "ID_DISCONNECTION_NOTIFICATION" << std::endl;
			break;
		case ID_CONNECTION_LOST:
			std::cout << "ID_CONNECTION_LOST" << std::endl;
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "ID_REMOTE_DISCONNECTION_NOTIFICATION" << std::endl;
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "ID_REMOTE_CONNECTION_LOST" << std::endl;
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			std::cout << "ID_REMOTE_NEW_INCOMING_CONNECTION" << std::endl;
			break;
		}
			////////////////////////////////////////////////
			// CUSTOM PACKETS                             
			////////////////////////////////////////////////
		case andrick_ID_CONNECTION_REQUEST_ACCEPTED:///Server sends this to client
		{
			std::cout << "andrick_ID_CONNECTION_REQUEST_ACCEPTED" << std::endl;
			ConnectionRequestAcceptedPacket* requestAcceptedPacket = (ConnectionRequestAcceptedPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionRequestAcceptedEvent>(packet->systemAddress, requestAcceptedPacket->newUserId));
			break;
		}
		case andrick_ID_CONNECTION_ATTEMPT_FAILED:///Server sends this to Client - Server rejected us for some reason
		{
			std::cout << "andrick_ID_CONNECTION_ATTEMPT_FAILED" << std::endl;
			ConnectionRequestFailedPacket* requestFailedPacket = (ConnectionRequestFailedPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionRequestFailedEvent>(packet->systemAddress, requestFailedPacket->errorMessage));
			break;
		}
		case andrick_ID_REQUEST_JOIN_SERVER:///Client sends this to Server
		{
			std::cout << "andrick_ID_REQUEST_JOIN_SERVER" << std::endl;
			RequestJoinServerPacket* requestJoinServerPacket = (RequestJoinServerPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionRequestJoinEvent>(requestJoinServerPacket->userId,
				requestJoinServerPacket->username, false, requestJoinServerPacket->userId));
			break;
		}
		case andrick_ID_JOIN_ACCEPTED:///Server sends this to Joining Client
		{
			std::cout << "andrick_ID_JOIN_ACCEPTED" << std::endl;
			JoinAcceptedPacket* joinAcceptedPacket = (JoinAcceptedPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionJoinAcceptedEvent>(joinAcceptedPacket->username, 
				joinAcceptedPacket->maxUserCount, joinAcceptedPacket->connectedUserCount));
			break;
		}
		case andrick_ID_JOIN_FAILED:///Server sends this to Joining Client
		{
			std::cout << "andrick_ID_JOIN_FAILED" << std::endl;
			JoinFailedPacket* joinFailedPacket = (JoinFailedPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionJoinFailedEvent>(joinFailedPacket->userId, joinFailedPacket->errorMessage));
			break;
		}
		case andrick_ID_USER_JOINED_SERVER:///Server sends this to existing Clients
		{
			std::cout << "andrick_ID_USER_JOINED_SERVER" << std::endl;
			NewUserJoinedServerPacket* userJoinedPacket = (NewUserJoinedServerPacket*)packet->data;
			newEvents.push_back(std::make_shared<ConnectionNewUserJoinedEvent>(userJoinedPacket->userId, userJoinedPacket->username));
			break;
		}
		case andrick_ID_USER_DISCONNECTED:
		{
			std::cout << "andrick_ID_USER_DISCONNECTED" << std::endl;
			UserDisconnectedPacket* userDisconnectedPacket = (UserDisconnectedPacket*)packet->data;
			newEvents.push_back(std::make_shared<UserDisconnectedEvent>(userDisconnectedPacket->userId));
			break;
		}
			////////////////////////////////////////////////
			// BOID PACKET TYPES (They all do the same thing rn, but I wanted the separate couts)                 
			////////////////////////////////////////////////
		case andrick_ID_BOID_DATA_PUSH_EVENT:
		{
			std::cout << "andrick_ID_BOID_DATA_PUSH_EVENT" << std::endl;
			BoidDataPacket* boidEvntPacket = (BoidDataPacket*)packet->data;
			newEvents.push_back(std::make_shared<BoidDataEvent>(boidEvntPacket->packetId, boidEvntPacket->boidColor, boidEvntPacket->boids, boidEvntPacket->senderId));
			break;
		}
		//case andrick_ID_BOID_DATA_SHARE_EVENT:
		//{
		//	std::cout << "andrick_ID_BOID_DATA_SHARE_EVENT" << std::endl;
		//	BoidDataPacket* boidEvntPacket = (BoidDataPacket*)packet->data;
		//	newEvents.push_back(std::make_shared<BoidDataEvent>(boidEvntPacket->packetId, boidEvntPacket->position, boidEvntPacket->velocity, boidEvntPacket->acceleration, boidEvntPacket->senderId));
		//	break;
		//}
		//case andrick_ID_BOID_DATA_COUPLE_EVENT:
		//{
		//	std::cout << "andrick_ID_BOID_DATA_COUPLE_EVENT" << std::endl;
		//	BoidDataPacket* boidEvntPacket = (BoidDataPacket*)packet->data;
		//	newEvents.push_back(std::make_shared<BoidDataEvent>(boidEvntPacket->packetId, boidEvntPacket->position, boidEvntPacket->velocity, boidEvntPacket->acceleration, boidEvntPacket->senderId));
		//	break;
		//}
			////////////////////////////////////////////////
			// UNKNOWN PACKETS                            
			////////////////////////////////////////////////
		default:
		{
			std::cout << "Unknown packet" << std::endl;
			break;
		}
		}

		for (auto iter = newEvents.begin(); iter != newEvents.end(); ++iter)
		{
			gEventSystem.queueLocalEvent(*iter);
		}
	}

	return packetsProcessed;
}

bool PacketHandler::broadcast(const char* packetData, std::size_t packetSize) const
{
	int success = mpPeer->Send(
		packetData, 
		(int)packetSize, 
		PacketPriority::IMMEDIATE_PRIORITY, 
		PacketReliability::RELIABLE_ORDERED,
		0, 
		RakNet::UNASSIGNED_SYSTEM_ADDRESS, 
		true);

	return success > 0;
}

bool PacketHandler::sendToOne(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress) const
{
	int success = mpPeer->Send(
		packetData, 
		(int)packetSize, 
		PacketPriority::IMMEDIATE_PRIORITY, 
		PacketReliability::RELIABLE_ORDERED,
		0, 
		ipAddress, 
		false);

	return success > 0;
}