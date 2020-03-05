#include <A3_DEMO/_andrick_Event/andrick_event.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packet.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

///Client -> Server - see if we can get inital connection
#pragma region NewIncomingConnectionEvent
void NewIncomingConnectionEvent::execute()
{
	std::cout << "New incoming connection..." << std::endl;

	//Check if we have room.
	UserId userId;
	std::string errorMessage;

	std::shared_ptr<SendableEvent> evnt;

	//Send an accepted/failed event to the client
	if (gDemoState->mpServer->processNewIncomingUser(clientAddress, userId, errorMessage))
		evnt = std::make_shared<ConnectionRequestAcceptedEvent>(gDemoState->mpPacketHandler->getServerAddress(), userId, false, userId);
	else
		evnt = std::make_shared<ConnectionRequestFailedEvent>(clientAddress, errorMessage);

	gEventSystem.queueNetworkEvent(evnt);
}
#pragma endregion

///Server -> Client - server got the connection and you can join
#pragma region ConnectionRequestAcceptedEvent
ConnectionRequestAcceptedEvent::ConnectionRequestAcceptedEvent(RakNet::SystemAddress serverAddress, UserId newUserId,
	bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::CONNECTION_REQUEST_ACCEPTED, EventProcessingType::CLIENTSIDE, isBroadcast, receiverId),
	serverAddress(serverAddress),
	newUserId(newUserId) {}

void ConnectionRequestAcceptedEvent::execute()
{
	gDemoState->mIsOnline = true;
	gDemoState->mpPacketHandler->setServerAddress(serverAddress);
	gDemoState->mpClient->setUserId(newUserId);
	std::cout << "Our connection request was accepted! UserId: " << std::to_string(newUserId) << std::endl;
}

std::size_t ConnectionRequestAcceptedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(ConnectionRequestAcceptedPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&ConnectionRequestAcceptedPacket(newUserId), packetSize);
	return packetSize;
}
#pragma endregion

///Server -> Client - server got the connection but it's full so bye
#pragma region ConnectionRequestFailedEvent
void ConnectionRequestFailedEvent::execute()
{
	std::cout << "Our connection request failed:\n\t" << errorMessage << std::endl;
}

std::size_t ConnectionRequestFailedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(ConnectionRequestFailedPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&ConnectionRequestFailedPacket(errorMessage), packetSize);
	return packetSize;
}
#pragma endregion

///Client -> Server - client asks if it can join with andrick specific 
///data (username, authority, etc)
#pragma region ConnectionRequestJoinEvent
ConnectionRequestJoinEvent::ConnectionRequestJoinEvent(UserId userId, const std::string& username,
	bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::CONNECTION_REQUEST_JOIN, EventProcessingType::SERVERSIDE, isBroadcast, receiverId),
	mUsername(username),
	mUserId(userId)
{

}

void ConnectionRequestJoinEvent::execute()
{
	if (gDemoState->mpServer->isUsernameTaken(mUsername))
	{
		std::cout << "USER NAME IS TAKEN" << std::endl;
		std::shared_ptr<SendableEvent> joinFailedEvnt = std::make_shared<ConnectionJoinFailedEvent>(mUserId,
			"Sorry, that username is already taken.");

		gEventSystem.queueNetworkEvent(joinFailedEvnt);
	}
	else
	{
		std::shared_ptr<Client> out;
		if (gDemoState->mpServer->getClientById(mUserId, out))
		{
			out->setUsername(mUsername);
		}

		std::shared_ptr<SendableEvent> privateEvnt = std::make_shared<ConnectionJoinAcceptedEvent>(mUsername,
			gDemoState->mpServer->getMaxUserCount(), gDemoState->mpServer->getConnectedUserCount(),
			false, mUserId);

		std::shared_ptr<SendableEvent> broadcastEvnt = std::make_shared<ConnectionNewUserJoinedEvent>(mUserId, mUsername);

		gEventSystem.queueNetworkEvent(privateEvnt);
		gEventSystem.queueNetworkEvent(broadcastEvnt);
	}
}

std::size_t ConnectionRequestJoinEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(RequestJoinServerPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&RequestJoinServerPacket(mUserId, mUsername), packetSize);
	return packetSize;
}
#pragma endregion

///Server -> Client - Everything is good, you can enter the server
#pragma region ConnectionJoinAcceptedEvent

ConnectionJoinAcceptedEvent::ConnectionJoinAcceptedEvent(const std::string& username, std::size_t maxUserCount, std::size_t connectedUserCount,
	bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::CONNECTION_JOIN_ACCEPTED, EventProcessingType::CLIENTSIDE, isBroadcast, receiverId),
	mUsername(username), mMaxUserCount(maxUserCount), mConnectedUserCount(connectedUserCount)
{
	///TODO: Take in an array of connected user data to populate mpClientMap
}

void ConnectionJoinAcceptedEvent::execute()
{
	gDemoState->mpClient->setServersMaxUserCount(mMaxUserCount);
	gDemoState->mpClient->setConnectedUserCount(mConnectedUserCount);
	gDemoState->mpClient->setUsername(mUsername);
	///TODO: Send over client data from server and initialize the mpClientMap

	std::cout << mUsername << " successfully joined the server! - " << std::to_string(mConnectedUserCount) << "/" << std::to_string(mMaxUserCount) << " online." << std::endl;
}

std::size_t ConnectionJoinAcceptedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(JoinAcceptedPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&JoinAcceptedPacket(mUsername, (char)mMaxUserCount, (char)mConnectedUserCount), packetSize);
	return packetSize;
}

#pragma endregion

#pragma region ConnectionJoinFailedEvent

ConnectionJoinFailedEvent::ConnectionJoinFailedEvent(UserId userId, const std::string& errorMessage) :
	SendableEvent(EventId::CONNECTION_JOIN_FAILED, EventProcessingType::BOTH, false, userId),
	userId(userId), errorMessage(errorMessage)
{

}

void ConnectionJoinFailedEvent::execute()
{
	if (gDemoState->mpPacketHandler->isServer())
	{
		if (gDemoState->mpServer->disconnectClient(userId))
		{
			std::cout << "Disconnecting client because they have an invalid username." << std::endl;
		}
	}
	else
	{
		std::cout << "You got disconnected by the server for having an invalid username." << std::endl;
		
		std::shared_ptr<SendableEvent> joinFailedEvnt = std::make_shared<ConnectionJoinFailedEvent>(userId,
			"Closing client connection.");

		gEventSystem.queueNetworkEvent(joinFailedEvnt);
	}
}

std::size_t ConnectionJoinFailedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(JoinFailedPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&JoinFailedPacket(userId, errorMessage), packetSize);
	return packetSize;
}

#pragma endregion


///Server -> All Clients - A new user joined the server
#pragma region ConnectionNewUserJoinedEvent

ConnectionNewUserJoinedEvent::ConnectionNewUserJoinedEvent(UserId user, const std::string& username,
	bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::CONNECTION_REQUEST_ACCEPTED, EventProcessingType::CLIENTSIDE, isBroadcast, receiverId),
	mUserId(user), mUsername(username)
{

}

void ConnectionNewUserJoinedEvent::execute()
{
	std::cout << "A new user has just joined the server. Don't be shy, say hi to " << mUsername << "!" << std::endl;
	
	if (mUserId != gDemoState->mpClient->getId())
	{
		gDemoState->mpClient->initNewUser(mUserId, mUsername);
	}
}

std::size_t ConnectionNewUserJoinedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(NewUserJoinedServerPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&NewUserJoinedServerPacket(mUserId, mUsername), packetSize);
	return packetSize;
}

#pragma endregion

#pragma region GenericEvent
std::size_t GenericEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(GenericEventPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&GenericEventPacket(packetId, userId), packetSize);
	return packetSize;
}
#pragma endregion

#pragma region CommandEvent
CommandEvent::CommandEvent(std::shared_ptr<Command> command, bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::COMMAND, EventProcessingType::BOTH, isBroadcast, receiverId),
	command(command) {}

void CommandEvent::execute() 
{ 
	command->runCommand(); 
}
#pragma endregion

#pragma region WhisperCommandEvent
WhisperCommandEvent::WhisperCommandEvent(std::shared_ptr<WhisperCommand> command) :
	CommandEvent(command, false, command->mpReciever->getId()) {}

std::size_t WhisperCommandEvent::allocatePacket(char*& out)
{
	std::shared_ptr<WhisperCommand> whisperCommandData = std::dynamic_pointer_cast<WhisperCommand>(command);
	std::size_t packetSize = sizeof(WhisperPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&WhisperPacket(whisperCommandData->mpSender->getId(), whisperCommandData->mpReciever->getId(), whisperCommandData->mMessage.c_str()), packetSize);
	return packetSize;
}
#pragma endregion

#pragma region UserDisconnectedEvent

UserDisconnectedEvent::UserDisconnectedEvent(UserId userId) :
	SendableEvent(EventId::USER_DISCONNECTED, EventProcessingType::BOTH, gDemoState->mpPacketHandler->isServer()),
	userId(userId)
{

}

void UserDisconnectedEvent::execute()
{
	std::cout << "User disconnected!" << std::endl;
	std::shared_ptr<SendableEvent> userDisconnectedEvnt = std::make_shared<UserDisconnectedEvent>(userId);
	gEventSystem.queueNetworkEvent(userDisconnectedEvnt);

	if (gDemoState->mpPacketHandler->isServer())
	{
		gDemoState->mpServer->disconnectClient(userId);
	}
	else if (gDemoState->mpClient->getId() == userId)
	{
		gDemoState->mpPacketHandler->disconnect();
	}
	else
	{
		///TODO: Use the clientside map. For now just use count until we send over user data.
		gDemoState->mpClient->setConnectedUserCount(gDemoState->mpClient->getConnectedUserCount() - 1);
	}
}

std::size_t UserDisconnectedEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(UserDisconnectedPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&UserDisconnectedPacket(userId), packetSize);
	return packetSize;
}

#pragma endregion
