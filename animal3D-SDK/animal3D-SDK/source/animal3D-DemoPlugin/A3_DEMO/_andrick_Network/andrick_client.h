#ifndef CLIENT_H_
#define CLIENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>

class Client : public EventListener
{
public:
	Client();
	virtual ~Client() = default;

	void processIncomingEvent(std::shared_ptr<struct Event> evnt) override;

	//These are all clientside functions.
	//The server has it's own copy of clients and
	//verifies userId/username/authority don't change
	//without permission.
	void setUserId(const UserId id);
	void setUsername(const std::string& username);
	void setAuthority(AuthorityId authority);
	void setAddress(const RakNet::SystemAddress& address) { mAddress = address; }

	bool getClientFromUsername(const std::string& username, std::shared_ptr<Client>& out);
	const UserId& getId() const;
	const std::string& getUsername() const;
	const AuthorityId& getAuthority() const;
	const RakNet::SystemAddress& getAddress() const { return mAddress; };

private:
	bool mIsConnected;
	UserId mUserId;
	std::string mUsername;
	AuthorityId mAuthority;
	RakNet::SystemAddress mAddress;
	std::map<UserId, std::shared_ptr<Client>> mClientMap;
};

#endif