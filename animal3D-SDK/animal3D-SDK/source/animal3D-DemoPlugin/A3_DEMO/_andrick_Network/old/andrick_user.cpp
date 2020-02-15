#include "andrick_user.h"

User::User(UserId id, const std::string username, AuthorityId authority, const RakNet::SystemAddress& address) :
	mId(id), 
	mUsername(username), 
	mAuthority(authority),
	mAddress(address)
{

}

std::string User::formatMessage(const char* username, const char* message, AuthorityId authority)
{
	std::string formattedMessage = "[";

	if (authority == AuthorityId::ADMIN)
	{
		formattedMessage += "** ";
	}

	formattedMessage += std::string(username) + "] " + std::string(message);
	return formattedMessage;
}

std::string User::formatMessage(const std::string& username, const std::string& message, AuthorityId authority)
{
	return formatMessage(username.c_str(), message.c_str(), authority);
}