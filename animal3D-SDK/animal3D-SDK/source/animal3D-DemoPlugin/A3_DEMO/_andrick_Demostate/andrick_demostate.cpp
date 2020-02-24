#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packet.h>

//a3i32 a3_DemoState::a3netStartup(a3ui16 port_inbound, a3ui16 port_outbound, a3ui16 maxConnect_inbound, a3ui16 maxConnect_outbound)
//{
//	if (!peer)
//	{
//		peer = RakNet::RakPeerInterface::GetInstance();
//		if (peer)
//		{
//			RakNet::SocketDescriptor socket[2] = 
//			{
//				RakNet::SocketDescriptor(port_inbound, 0),		// server settings
//				RakNet::SocketDescriptor(),//port_outbound, 0),	// client settings
//			};
//
//			// choose startup settings based on 
//			a3boolean const isServer = port_inbound && maxConnect_inbound;
//			a3boolean const isClient = port_outbound && maxConnect_outbound;
//			if (peer->Startup(maxConnect_inbound + maxConnect_outbound, socket + isClient, isServer + isClient) == RakNet::StartupResult::RAKNET_STARTED)
//			{
//				peer->SetMaximumIncomingConnections(maxConnect_inbound);
//				peer->SetOccasionalPing(true);
//
//				this->port_inbound = port_inbound;
//				this->port_outbound = port_outbound;
//				this->maxConnect_inbound = maxConnect_inbound;
//				this->maxConnect_outbound = maxConnect_outbound;
//
//				return 1;
//			}
//		}
//	}
//	return 0;
//}

// shutdown networking
//a3i32 a3_DemoState::a3netShutdown()
//{
//	if (peer)
//	{
//		//peer = (RakNet::RakPeerInterface*)peer;
//		RakNet::RakPeerInterface::DestroyInstance(peer);
//		peer = 0;
//		return 1;
//	}
//	return 0;
//}

// connect
//a3i32 a3_DemoState::a3netConnect(a3netAddressStr const ip)
//{
//	if (peer)
//	{
//		peer->Connect(ip, port_outbound, 0, 0);
//		return 1;
//	}
//	return 0;
//}

// disconnect
//a3i32 a3_DemoState::a3netDisconnect()
//{
//	if (peer)
//	{
//		a3ui16 i, j = peer->NumberOfConnections();
//		for (i = 0; i < j; ++i)
//			peer->CloseConnection(peer->GetSystemAddressFromIndex(i), true);
//		return 1;
//	}
//	return 0;
//}

// process inbound packets
//a3i32 a3_DemoState::a3netProcessInbound()
//{
//	if (peer)
//	{
//		RakNet::Packet* packet;
//		RakNet::MessageID msg;
//		a3i32 count = 0;
//
//		for (packet = peer->Receive();
//			packet;
//			peer->DeallocatePacket(packet), packet = peer->Receive(), ++count)
//		{
//			RakNet::BitStream bs_in(packet->data, packet->length, false);
//			bs_in.Read(msg);
//			switch (msg)
//			{
//				// check for timestamp and process
//			case ID_TIMESTAMP:
//				bs_in.Read(msg);
//				// ****TO-DO: handle timestamp
//
//				// do not break; proceed to default case to process actual message contents
//			default:
//				switch (msg)
//				{
//				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
//					printf("Another client has disconnected.\n");
//					break;
//				case ID_REMOTE_CONNECTION_LOST:
//					printf("Another client has lost the connection.\n");
//					break;
//				case ID_REMOTE_NEW_INCOMING_CONNECTION:
//					printf("Another client has connected.\n");
//					break;
//				case ID_CONNECTION_REQUEST_ACCEPTED:
//					printf("Our connection request has been accepted.\n");
//					if (!isServer)
//					{
//						//setup self
//						serverAddress = packet->systemAddress;
//					}
//					break;
//				case ID_NEW_INCOMING_CONNECTION:
//					printf("A connection is incoming.\n");
//					if (isServer)
//					{
//						mUserAddressList.push_back(packet->systemAddress);
//
//						JoinAcceptedPacket joinAcceptedData = JoinAcceptedPacket(
//							'0',
//							"None",
//							10,
//							2
//						);
//
//						sendOncePacket((const char*)(&joinAcceptedData), sizeof(JoinAcceptedPacket), packet->systemAddress);
//
//						//RakNet::BitStream bsOut[1];
//						//bsOut->Write((RakNet::MessageID)ID_CONNECTION_REQUEST_ACCEPTED);
//						//bsOut->Write("Hello world");
//						//peer->Send(bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//					}
//					break;
//				case ID_NO_FREE_INCOMING_CONNECTIONS:
//					printf("The server is full.\n");
//					break;
//				case ID_DISCONNECTION_NOTIFICATION:
//					if (maxConnect_outbound) {
//						printf("A client has disconnected.\n");
//					}
//					else {
//						printf("We have been disconnected.\n");
//					}
//					break;
//				case ID_CONNECTION_LOST:
//					if (maxConnect_outbound) {
//						printf("A client lost the connection.\n");
//					}
//					else {
//						printf("Connection lost.\n");
//					}
//					break;
//
//				case BASIC_EVENT:
//					{
//						BasicEventPacket * data = (BasicEventPacket*)(packet->data);
//
//						printf("DEBUG MESSAGE: received packet BASIC_EVENT.\n");
//						if (isServer)
//						{
//							broadcastPacket((const char*)data, sizeof(BasicEventPacket));
//						}
//						else
//						{
//							//Add to local queue
//							std::shared_ptr<BasicEvent> evnt;
//							evnt = std::make_shared<BasicEvent>(data->ID);
//
//							gEventSystem.queueLocalEvent(evnt);
//						}
//					}
//					break;
//
//				default:
//					printf("Message with identifier %i has arrived.\n", msg);
//					break;
//				}
//				break;
//			}
//		}
//		return count;
//	}
//	return 0;
//}

//void a3_DemoState::broadcastPacket(const char* packetData, std::size_t packetSize)
//{
//	peer->Send(packetData, (int)packetSize,
//		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
//		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
//}
//
//void a3_DemoState::sendOncePacket(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress)
//{
//	peer->Send(packetData, (int)packetSize,
//		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
//		0, ipAddress, false);
//}