#include <stdio.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>
#include <string>

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

	//Find out if this is a client or server instance.
	std::string clientServerInput;
	getInput("(C) or (S)erver?\n", clientServerInput);

	//Default to client if invalid input.
	const char csResult = clientServerInput.empty() ? 'c' : clientServerInput[0];

	//Server port is always initialized for server and client.
	std::string serverPortInput;
	getInput("Server Port: ", serverPortInput);

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

	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
