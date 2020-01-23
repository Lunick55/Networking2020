#ifndef USER_H_
#define USER_H_

#include "IPacketSender.h"
#include "IPacketReceiver.h"

class User
{
public:
	explicit User(UserId id, const std::string username, AuthorityId authority, const RakNet::SystemAddress& address);
	virtual ~User() = default;

	//Checks with the server if this is a valid request.
	//If not, then it returns false.
	//If yes, then it sets the User's authority and returns true.
	//bool requestAuthority(AuthorityId authority);

	inline const UserId& getUserId() const { return mId; }
	inline const AuthorityId& getAuthority() const { return mAuthority; }
	inline const RakNet::SystemAddress& getAddress() const { return mAddress; }
	inline const std::string& getUsername() const { return mUsername; }

private:
	UserId mId;
	AuthorityId mAuthority;
	RakNet::SystemAddress mAddress;
	std::string mUsername;
};

#endif