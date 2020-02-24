#ifndef SERVER_H_
#define SERVER_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>

class Server : public EventListener
{
public:
	Server();
	~Server() = default;

	void processIncomingEvent(std::shared_ptr<struct Event> evnt) override;

private:
	static UserId sUserIdCounter;
	std::map<UserId, std::shared_ptr<Client>> mConnectedUserMap;
};

#endif