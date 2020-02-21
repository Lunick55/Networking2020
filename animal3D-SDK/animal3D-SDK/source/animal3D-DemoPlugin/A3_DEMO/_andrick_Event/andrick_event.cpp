#include <A3_DEMO/_andrick_Event/andrick_event.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packet.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>

CommandEvent::CommandEvent(std::shared_ptr<Command> command) :
	Event(EventId::COMMAND),
	command(command) 
{}

//Used on the client to execute the command.
void CommandEvent::execute()
{
	command->runCommand();
}


WhisperCommandEvent::WhisperCommandEvent(std::shared_ptr<WhisperCommand> command) :
	CommandEvent(command) 
{}

std::size_t WhisperCommandEvent::allocatePacket(char*& out)
{
	std::shared_ptr<WhisperCommand> whisperCommandData = std::dynamic_pointer_cast<WhisperCommand>(command);
	std::size_t packetSize = sizeof(WhisperPacket);

	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&WhisperPacket(whisperCommandData->mpSender->getId(), whisperCommandData->mpReciever->getId(), whisperCommandData->mMessage.c_str()), packetSize);

	return packetSize;
}

std::size_t BasicEvent::allocatePacket(char*& out)
{
	//Get amount of memory we need to allocate
	std::size_t packetSize = sizeof(BasicEventPacket);

	//Create that memory dynamically and copy it to our out variable (whoever calls this function is required to free this)
	out = (char*)malloc(packetSize);
	memcpy(out, (char*)&BasicEventPacket(ID), packetSize);
	
	//return the packet size
	return packetSize;
}

