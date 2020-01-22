#include "User.h"

User::User(UserId id, const std::string username, AuthorityId authority, const RakNet::SystemAddress& address) :
	mId(id), 
	mUsername(username), 
	mAuthority(authority),
	mAddress(address)
{

}

void User::hostChatroom()
{

}

void User::connectToChatroom(const std::string& address)
{

}

//Checks with the server if this is a valid request.
//If not, then it returns false.
//If yes, then it sets the User's authority and returns true.
bool User::requestAuthority(AuthorityId authority)
{
	return false;
}

void User::sendPacket(const Packet& packet)
{

}

//Check if a packet is incoming
void User::receivePacket()
{

}