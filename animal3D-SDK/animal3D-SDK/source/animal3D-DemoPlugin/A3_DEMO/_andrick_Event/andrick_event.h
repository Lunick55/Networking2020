#ifndef EVENT_H_
#define EVENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

enum class EventDispatchType : char
{
	UNDEFINED = -1,
	LOCAL,
	NETWORK
};

enum class EventProcessingType : char
{
	SERVERSIDE,
	CLIENTSIDE,
	BOTH
};

struct Event
{
	Event(EventId id, EventProcessingType processType = EventProcessingType::BOTH) : 
		eventId(id),
		processingType(processType),
		dispatchType(EventDispatchType::UNDEFINED)
	{}

	virtual ~Event() = default;

	virtual void execute() {};
	virtual std::size_t allocatePacket(char*& out) = 0;

	const EventId eventId;
	EventProcessingType processingType;
	EventDispatchType dispatchType;
};

//An event that gets fired when a client receives a connection accepted packet.
struct ConnectionRequestAcceptedEvent : public Event
{
	ConnectionRequestAcceptedEvent(RakNet::SystemAddress serverAddress);
	virtual ~ConnectionRequestAcceptedEvent() = default;

	virtual std::size_t allocatePacket(char*& out) override;

	RakNet::SystemAddress serverAddress;
};

struct BasicEvent : public Event
{
	BasicEvent(EventId id);
	virtual ~BasicEvent() = default;
	virtual std::size_t allocatePacket(char*& out) override;
};

struct CommandEvent : public Event
{
	CommandEvent(std::shared_ptr<struct Command> command);
	virtual ~CommandEvent() = default;

	//Used on the client to execute the command.
	virtual void execute() override;

	//Used for packaging up the event and sending the data over the network.
	virtual std::size_t allocatePacket(char*& out) override = 0;

	std::shared_ptr<struct Command> command;
};

struct WhisperCommandEvent : public CommandEvent
{
	WhisperCommandEvent(std::shared_ptr<struct WhisperCommand> command);
	virtual ~WhisperCommandEvent() = default;

	virtual std::size_t allocatePacket(char*& out) override;
};

#endif