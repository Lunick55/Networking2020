#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

PacketHandler::PacketHandler(bool isServer) :
	mIsServer(isServer),
	mMaxConnections(0),
	mpPacket(nullptr),
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
		RakNet::RakPeerInterface::DestroyInstance(mpPeer);
		mpPeer = nullptr;

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

		return true;
	}

	return false;
}

int PacketHandler::processInboundPackets()
{
	RakNet::Packet* packet;
	RakNet::MessageID messageId;
	int packetsProcessed = 0;

	for (packet = mpPeer->Receive(); 
		packet != nullptr; 
		mpPeer->DeallocatePacket(packet), packet = mpPeer->Receive(), ++packetsProcessed)
	{
		RakNet::BitStream inBitStream(packet->data, packet->length, false);
		inBitStream.Read(messageId);

		std::shared_ptr<Event> evnt;

		switch (messageId)
		{
		case ID_TIMESTAMP:
			inBitStream.Read(messageId);
			//Todo: handle timestamp
		default:
			switch (messageId)
			{
				////////////////////
				// RAKNET PACKETS //
				////////////////////
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				break;
			}
			case ID_NEW_INCOMING_CONNECTION:
				///This event should only ever get caught on the server.
				evnt = std::make_shared<ConnectionRequestAcceptedEvent>(packet->systemAddress);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				break;
			case ID_CONNECTION_LOST:
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				break;
			case ID_REMOTE_CONNECTION_LOST:
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				break;

				////////////////////
				// CUSTOM PACKETS //
				////////////////////

			case andrick_ID_REQUEST_JOIN_SERVER:///Client -> Server
				break;

			case andrick_ID_JOIN_ACCEPTED:
				break;

			case andrick_ID_USER_JOINED_SERVER:
				break;

			case andrick_ID_BASIC_EVENT:
				break;

				/////////////////////
				// UNKNOWN PACKETS //
				/////////////////////
			default:
				break;
			}
		}

		if (evnt != nullptr)
		{
			gEventSystem.queueLocalEvent(evnt);
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