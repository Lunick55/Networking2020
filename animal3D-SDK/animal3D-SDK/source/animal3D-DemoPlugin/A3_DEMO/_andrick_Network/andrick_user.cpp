#include <A3_DEMO/_andrick_Network/andrick_user.h>

UserId User::sUserIdCounter = 0;

User::User(const std::string& name, AuthorityId authority) :
	mID(sUserIdCounter++),
	mUSERNAME(name),
	mAuthority(authority)
{

}

const UserId& User::getId() const
{
	return mID;
}

const std::string& User::getUsername() const
{
	return mUSERNAME;
}

const AuthorityId& User::getAuthority() const
{
	return mAuthority;
}