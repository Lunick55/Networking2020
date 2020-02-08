#ifndef USER_H_
#define USER_H_

#include "../_andrick_Utils/andrick_common.h"

class User
{
public:
	explicit User(UserId id, const std::string username, AuthorityId authority, const RakNet::SystemAddress& address);
	virtual ~User() = default;

	inline const UserId& getUserId() const { return mId; }
	inline const AuthorityId& getAuthority() const { return mAuthority; }
	inline const RakNet::SystemAddress& getAddress() const { return mAddress; }
	inline const std::string& getUsername() const { return mUsername; }

	static std::string formatMessage(const char* username, const char* message, AuthorityId authority);
	static std::string formatMessage(const std::string& username, const std::string& message, AuthorityId authority);

private:
	UserId mId;
	AuthorityId mAuthority;
	RakNet::SystemAddress mAddress;
	std::string mUsername;
};

#endif