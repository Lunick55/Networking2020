#include <A3_DEMO/_andrick_Event/andrick_event.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packet.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

#pragma region ConnectionRequestAcceptedEvent
ConnectionRequestAcceptedEvent::ConnectionRequestAcceptedEvent(RakNet::SystemAddress serverAddress, UserId newUserId,
	bool isBroadcast, UserId receiverId) :
	SendableEvent(EventId::CONNECTION_REQUEST_ACCEPTED, EventProcessingType::CLIENTSIDE, isBroadcast, receiverId),
	serverAddress(serverAddress),
	newUserId(newUserId) {}

void ConnectionRequestAcceptedEvent::execute()
{
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
		evnt = std::make_shared<ConnectionRequestFailedEvent>(errorMessage, false, userId);

	gEventSystem.queueNetworkEvent(evnt);
}
#pragma endregion

#pragma region GenericEvent
std::size_t GenericEvent::allocatePacket(char*& out)
{
	std::size_t packetSize = sizeof(BasicEventPacket);
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&BasicEventPacket(eventId), packetSize);
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
