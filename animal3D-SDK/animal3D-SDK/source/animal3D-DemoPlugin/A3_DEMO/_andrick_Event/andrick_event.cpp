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

PacketEventId WhisperCommandEvent::generatePacket(const char*& packetData)
{
	//Convert to derived class
	std::shared_ptr<WhisperCommand> whisperCommandData = std::dynamic_pointer_cast<WhisperCommand>(command);

	//Generate packet with command args
	WhisperPacket packet = WhisperPacket(whisperCommandData->mpSender->getId(), whisperCommandData->mpReciever->getId(), whisperCommandData->mMessage.c_str());

	//Convert to packet data
	packetData = (const char*)&packet;

	//return the packet id
	return packet.packetId;
}
