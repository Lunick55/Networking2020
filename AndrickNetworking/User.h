#ifndef USER_H_
#define USER_H_

#include "IPacketSender.h"

class User : IPacketSender
{
public:
	User();
	~User() = default;

	void hostChatroom();
	void connectToChatroom(const std::string& address);

	//Checks with the server if this is a valid request.
	//If not, then it returns false.
	//If yes, then it sets the User's authority and returns true.
	bool requestAuthority(AuthorityId authority);

	virtual void sendPacket(const Packet& packet) override;

	inline const UserId& getUserId() const { return mId; }
	inline const AuthorityId& getAuthority() const { return mAuthority; }
	inline const RakNet::SystemAddress& getAddress() const { return mUserAddress; }

private:
	UserId mId;
	AuthorityId mAuthority;
	RakNet::SystemAddress mUserAddress;
};

#endif