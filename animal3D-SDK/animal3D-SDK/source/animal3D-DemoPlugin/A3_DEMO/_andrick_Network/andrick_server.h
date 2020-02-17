#ifndef SERVER_H_
#define SERVER_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>

class Server
{
public:
	Server() = default;
	~Server() = default;

private:
	static UserId sUserIdCounter;
	RakNet::SystemAddress mServerAddress;
	std::map<UserId, std::shared_ptr<Client>> mConnectedUserMap;
};

#endif