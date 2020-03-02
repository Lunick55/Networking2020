#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

Client::Client() :
	mIsConnected(false),
	mAuthority(AuthorityId::NORMAL),
	mUserId(-1),
	mServersMaxUserCount(0),
	mClientMap({})
{}

void Client::setUserId(const UserId id)
{
	mUserId = id;
}

void Client::setUsername(const std::string& username)
{
	mUsername = username;
}

void Client::setAuthority(AuthorityId authority)
{
	mAuthority = authority;
}

const UserId& Client::getId() const
{
	return mUserId;
}

const std::string& Client::getUsername() const
{
	return mUsername;
}

const AuthorityId& Client::getAuthority() const
{
	return mAuthority;
}

void Client::processIncomingEvent(std::shared_ptr<struct Event> evnt)
{
	///We can just use execute() in the future if necessary, but for now, switch is fine.

	switch (evnt->eventId)
	{
	case EventId::CONNECTION_REQUEST_ACCEPTED:
	{
		evnt->execute();
		break;
	}
	case EventId::CONNECTION_REQUEST_FAILED:
	{
		evnt->execute();
		break;
	}
	default:
		break;
	}
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