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
	bool processNewIncomingUser(RakNet::SystemAddress clientAddress, UserId& newUserId, std::string& errorMessage);

	bool getClientById(UserId userId, std::shared_ptr<Client>& out);

	inline void setMaxUsers(std::size_t maxUsers) { mMaxUsers = maxUsers; };
	inline std::size_t getMaxUserCount() const { return mMaxUsers; };
	inline std::size_t getConnectedUserCount() const { return mConnectedUserMap.size(); };

private:
	std::size_t mMaxUsers;
	static UserId sUserIdCounter;
	std::map<UserId, std::shared_ptr<Client>> mConnectedUserMap;
};

#endif