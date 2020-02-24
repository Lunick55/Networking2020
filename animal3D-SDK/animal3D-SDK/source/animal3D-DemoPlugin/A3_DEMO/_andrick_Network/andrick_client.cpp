#include <A3_DEMO/_andrick_Network/andrick_client.h>

Client::Client(const UserId id, const std::string& name, AuthorityId authority) :
	mID(id),
	mUSERNAME(name),
	mAuthority(authority)
{

}

const UserId& Client::getId() const
{
	return mID;
}

const std::string& Client::getUsername() const
{
	return mUSERNAME;
}

const AuthorityId& Client::getAuthority() const
{
	return mAuthority;
}

void Client::processIncomingEvent(std::shared_ptr<struct Event> evnt)
{

}

bool Client::getClientFromUsername(const std::string& username, std::shared_ptr<Client>& out)
{
	bool success = false;
	out = nullptr;

	for (auto iter = mClientMap.begin(); iter != mClientMap.end(); ++iter)
	{
		if (iter->second->getUsername().compare(username) == 0)
		{
			out = iter->second;
			success = true;
			break;
		}
	}

	return success;
}