#include <stdio.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>
#include <string>
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"

#pragma pack(push, 1)
struct Message
{
	unsigned char typeId;//your type here

	//the message from the user
	std::string message;
	//more data goes here
};
#pragma pack(pop)

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

void getInput(const char* outputText, std::string& input)
{
	printf(outputText);
	std::getline(std::cin, input);
}

int main(void)
{
	unsigned int maxClients = 10;
	unsigned short serverPort = 600;

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

		try
		{
			maxClients = std::stoi(maxClientInput);
		}
		catch (const std::exception& e)
		{
			std::cout << "Could not convert Max Client input into a number: " << e.what() << std::endl;
			return -1;
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

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				RakNet::BitStream bsOut;
				Message messageOut;

				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				messageOut.typeId = ID_GAME_MESSAGE_1;

				bsOut.Write("Hello world");
				messageOut.message = "SUCK MY TITS SLUT";
	
				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				peer->Send((char*)&messageOut, sizeof(Message), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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
			case ID_GAME_MESSAGE_1:
			{
				Message* m = (Message*)packet->data;
				unsigned char* dataThing = packet->data;

				assert(packet->length == sizeof(Message)); // This is a good idea if you’re transmitting structs.
				if (packet->length != sizeof(Message))
				{
					std::cout << "Length is the big suck" << std::endl;
					return 0;
				}

				//RakNet::RakString rs;
				//RakNet::BitStream bsIn(packet->data, packet->length, false);
				//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				//bsIn.Read(rs);

				//this crashes
				std::cout << m->message << std::endl;
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}