#include <stdio.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>
#include <string>
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"

#pragma pack(push, 1)
struct AndrickPacket
{
	unsigned char packetId;
	const char* message;
};
#pragma pack(pop)

enum GameMessages
{
	ID_GAME_MESSAGE = ID_USER_PACKET_ENUM
};

void getInput(const char* outputText, std::string& input)
{
	printf(outputText);
	std::getline(std::cin, input);
}

int main(void)
{
	unsigned int maxClients = 10;
	unsigned short serverPort = 6000;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet* packet;

	//Find out if this is a client or server instance.
	std::string clientServerInput;
	getInput("(C) or (S)erver?\n", clientServerInput);

	//Default to client if invalid input.
	const char csResult = clientServerInput.empty() ? 'c' : clientServerInput[0];

	//Server port is always initialized for server and client.
	std::string serverPortInput;
	getInput("Server Port: ", serverPortInput);

	if (!serverPortInput.empty())
	{
		try
		{
			serverPort = stoi(serverPortInput);
		}
		catch (const std::exception & e)
		{
			std::cout << "Could not convert ServerPort input into a number: " << e.what() << std::endl;
			return -1;
		}
	}

	//Client
	if (std::tolower(csResult) == 'c')
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	//Server
	else 
	{
		//Max Clients is only initialized if server is being used.
		std::string maxClientInput;
		getInput("Max Clients: ", maxClientInput);

		if (!maxClientInput.empty())
		{
			try
			{
				maxClients = std::stoi(maxClientInput);
			}
			catch (const std::exception & e)
			{
				std::cout << "Could not convert Max Client input into a number: " << e.what() << std::endl;
				return -1;
			}
		}

		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else 
	{
		std::cout << ("Enter server IP or hit enter for 127.0.0.1\n");
		std::string serverIPInput;
		getInput("Server IP: ", serverIPInput);

		if (serverIPInput.empty()) 
		{
			serverIPInput = "127.0.0.1";
		}

		printf("Starting the client.\n");

		peer->Connect(serverIPInput.c_str(), serverPort, 0, 0);
	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				AndrickPacket sendingPacket;
				sendingPacket.packetId = ID_GAME_MESSAGE;
				sendingPacket.message = "Hello World!\0";

				peer->Send(
					(const char*)(&sendingPacket),
					sizeof(AndrickPacket),
					PacketPriority::IMMEDIATE_PRIORITY,
					PacketReliability::RELIABLE_ORDERED,
					0,
					packet->systemAddress,
					true
				);
			}
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) 
				{
					printf("A client has disconnected.\n");
				}
				else 
				{
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) 
				{
					printf("A client lost the connection.\n");
				}
				else 
				{
					printf("Connection lost.\n");
				}
				break;
			case ID_GAME_MESSAGE:
			{
				AndrickPacket* data = (AndrickPacket*)packet->data;

				assert(packet->length == sizeof(AndrickPacket));
				if (packet->length != sizeof(AndrickPacket))
				{
					std::cout << "Uh oh! Invalid length for AndrickPacket!" << std::endl;
					return -1;
				}

				std::cout << "Incoming message: " << std::string(data->message) << std::endl;
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}