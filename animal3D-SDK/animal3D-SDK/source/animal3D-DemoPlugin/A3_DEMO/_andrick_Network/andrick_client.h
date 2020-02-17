#ifndef CLIENT_H_
#define CLIENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class Client
{
public:
	Client(const UserId id, const std::string& name, AuthorityId authority);
	virtual ~Client() = default;

	bool getClientFromUsername(const std::string& username, std::shared_ptr<Client>& out);
	const UserId& getId() const;
	const std::string& getUsername() const;
	const AuthorityId& getAuthority() const;

private:
	const UserId mID;
	const std::string mUSERNAME;
	AuthorityId mAuthority;
	RakNet::SystemAddress mLocalAddress;
	std::map<UserId, std::shared_ptr<Client>> mClientMap;

};

#endif