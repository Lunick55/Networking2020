#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

UserId Server::sUserIdCounter = 0;

Server::Server()
{

}

void Server::processIncomingEvent(std::shared_ptr<struct Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::NEW_INCOMING_CONNECTION:
		std::cout << "A new user is attempting to connect!" << std::endl;
		evnt->execute();
		break;
	default:
		break;
	}
}

bool Server::processNewIncomingUser(RakNet::SystemAddress clientAddress, UserId& newUserId, std::string& errorMessage)
{
	if (mConnectedUserMap.size() < gDemoState->mpPacketHandler->getMaxConnections())
	{
		std::shared_ptr<Client> newUser = std::make_shared<Client>();
		newUser->setAddress(clientAddress);
		newUser->setUserId(sUserIdCounter++);
		newUserId = newUser->getId();
		newUser->setUserId(newUserId);
		newUser->setUsername("Noob" + std::to_string(newUserId));
		newUser->setAuthority(AuthorityId::NORMAL);
		mConnectedUserMap.insert({ newUserId, newUser });
		std::cout << "Adding new user to server map: " << newUser->getUsername() << std::endl;
		return true;
	}
	else
	{
		errorMessage = "The server is currently full! Try again later. :(";
		newUserId = -1;
	}

	return false;
}

bool Server::getClientById(UserId userId, std::shared_ptr<Client>& out)
{
	std::map<UserId, std::shared_ptr<Client>>::iterator iter = mConnectedUserMap.find(userId);
	if (iter != mConnectedUserMap.end())
	{
		out = iter->second;
		return true;
	}

	return false;
}