#ifndef USER_H_
#define USER_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class User
{
public:
	User(const std::string& name, AuthorityId authority);
	virtual ~User() = default;

	const UserId& getId() const;
	const std::string& getUsername() const;
	const AuthorityId& getAuthority() const;

private:
	static UserId sUserIdCounter;

	const UserId mID;
	const std::string mUSERNAME;
	AuthorityId mAuthority;
};

#endif