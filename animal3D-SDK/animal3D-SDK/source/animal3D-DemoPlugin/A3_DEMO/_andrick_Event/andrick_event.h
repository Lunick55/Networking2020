#ifndef EVENT_H_
#define EVENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

enum class EventDispatchType : char
{
	UNDEFINED = -1,
	LOCAL,
	NETWORK
};

struct Event
{
	Event(EventId id) : 
		ID(id), dispatchType(EventDispatchType::UNDEFINED) {}

	virtual ~Event() = default;

	virtual void execute() = 0;
	virtual PacketEventId generatePacket(const char*& packetData) = 0;

	const EventId ID;
	EventDispatchType dispatchType;
};

struct CommandEvent : public Event
{
	CommandEvent(std::shared_ptr<struct Command> command);
	virtual ~CommandEvent() = default;

	//Used on the client to execute the command.
	virtual void execute();

	//Used for packaging up the event and sending the data over the network.
	virtual PacketEventId generatePacket(const char*& packetData) = 0;

	std::shared_ptr<struct Command> command;
};

struct WhisperCommandEvent : public CommandEvent
{
	WhisperCommandEvent(std::shared_ptr<struct WhisperCommand> command);
	virtual ~WhisperCommandEvent() = default;

	virtual PacketEventId generatePacket(const char*& packetData);
};

#endif