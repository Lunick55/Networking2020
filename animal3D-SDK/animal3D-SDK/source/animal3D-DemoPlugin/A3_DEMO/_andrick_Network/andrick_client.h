#ifndef CLIENT_H_
#define CLIENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>

class Client : public EventListener
{
public:
	Client(const UserId id, const std::string& name, AuthorityId authority);
	virtual ~Client() = default;

	void processIncomingEvent(std::shared_ptr<struct Event> evnt) override;

	bool getClientFromUsername(const std::string& username, std::shared_ptr<Client>& out);

	const UserId& getId() const;
	const std::string& getUsername() const;
	const AuthorityId& getAuthority() const;

private:
	const UserId mID;
	const std::string mUSERNAME;
	AuthorityId mAuthority;
	std::map<UserId, std::shared_ptr<Client>> mClientMap;
};

#endif