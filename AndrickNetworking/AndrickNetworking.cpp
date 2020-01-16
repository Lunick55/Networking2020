#include <stdio.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>
#include <string>

void getInput(const char* outputText, std::string input)
{
	printf(outputText);
	std::getline(std::cin, input);
}

int main(void)
{
	const int MAX_COUNT = 512;
	unsigned int maxClients = 0;
	unsigned short serverPort = 7777;

	char str[MAX_COUNT];
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	getInput("(C) or (S)erver?\n", str);

	//Max Clients is only initialized if server is being used.
	std::string maxClientInput;
	getInput("Max Clients: ", maxClientInput);

	//Server port is always initialized for server and client.
	std::string serverPortInput;
	getInput("Server Port: ", serverPortInput);

	fgets(str, MAX_COUNT, stdin);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}


	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
