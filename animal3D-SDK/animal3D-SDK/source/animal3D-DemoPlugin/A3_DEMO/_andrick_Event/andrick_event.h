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
	inline Event(EventId id, EventProcessingType processType = EventProcessingType::BOTH) : 
		eventId(id),
		processingType(processType),
		dispatchType(EventDispatchType::UNDEFINED) {}
	virtual ~Event() = default;
	virtual void execute() {};

	inline virtual std::size_t allocatePacket(char*& out)
	{
		//By default, events that don't/can't be sent over the network
		//default to a size of 0.
		out = nullptr;
		return 0;
	}

	const EventId eventId;
	EventProcessingType processingType;
	EventDispatchType dispatchType;
};

struct SendableEvent : public Event
{
	inline SendableEvent(EventId id, EventProcessingType processType = EventProcessingType::BOTH, bool isBroadcast = false, UserId receiverId = -1) :
		Event(id),
		isBroadcast(isBroadcast),
		receiverId(receiverId) {}
	virtual ~SendableEvent() = default;

	virtual std::size_t allocatePacket(char*& out) = 0;//Used for packaging up the event and sending the data over the network.

	bool isBroadcast;
	UserId receiverId;
};

struct NewIncomingConnectionEvent : public Event
{
	inline NewIncomingConnectionEvent(RakNet::SystemAddress clientAddress) :
		Event(EventId::NEW_INCOMING_CONNECTION, EventProcessingType::SERVERSIDE),
		clientAddress(clientAddress) {}
	virtual ~NewIncomingConnectionEvent() = default;
	virtual void execute() override;

	RakNet::SystemAddress clientAddress;
};

//An event that gets fired when a client receives a connection accepted packet.
struct ConnectionRequestAcceptedEvent : public SendableEvent
{
	ConnectionRequestAcceptedEvent(RakNet::SystemAddress serverAddress, UserId newUserId,
		bool isBroadcast = false, UserId receiverId = -1);

	virtual ~ConnectionRequestAcceptedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	RakNet::SystemAddress serverAddress;
	UserId newUserId;
};

struct ConnectionRequestFailedEvent : public SendableEvent
{
	inline ConnectionRequestFailedEvent(RakNet::SystemAddress clientAddress, const std::string& errorMessage) :
		SendableEvent(EventId::CONNECTION_REQUEST_FAILED, EventProcessingType::CLIENTSIDE),
		errorMessage(errorMessage) {}

	virtual ~ConnectionRequestFailedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	RakNet::SystemAddress clientAddress;
	std::string errorMessage;
};

struct ConnectionRequestJoinEvent : public SendableEvent
{
	ConnectionRequestJoinEvent(UserId userId, const std::string& username,
		bool isBroadcast = false, UserId receiverId = -1);

	virtual ~ConnectionRequestJoinEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	UserId mUserId;
	std::string mUsername;
};

struct ConnectionJoinAcceptedEvent : public SendableEvent
{
	ConnectionJoinAcceptedEvent(const std::string& username, std::size_t maxUserCount, std::size_t connectedUserCount,
		bool isBroadcast = false, UserId receiverId = -1);

	virtual ~ConnectionJoinAcceptedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	std::string mUsername;
	std::size_t mMaxUserCount;
	std::size_t mConnectedUserCount;
};

struct ConnectionJoinFailedEvent : public SendableEvent
{
	ConnectionJoinFailedEvent(UserId userId, const std::string& errorMessage);

	virtual ~ConnectionJoinFailedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	UserId userId;
	std::string errorMessage;
};

struct ConnectionNewUserJoinedEvent : public SendableEvent
{
	ConnectionNewUserJoinedEvent(UserId user, const std::string& username,
		bool isBroadcast = true, UserId receiverId = -1);

	virtual ~ConnectionNewUserJoinedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	UserId mUserId;
	std::string mUsername;
};

//struct GenericEvent : public SendableEvent
//{
//	inline GenericEvent(PacketEventId packetId, UserId userId = -1, bool isBroadcast = false, UserId receiverId = -1) :
//		SendableEvent(EventId::GENERIC_EVENT, EventProcessingType::BOTH, isBroadcast, receiverId),
//		packetId(packetId), userId(userId) {}
//	virtual ~GenericEvent() = default;
//	virtual std::size_t allocatePacket(char*& out) override;
//
//	UserId userId;
//	PacketEventId packetId;
//};

struct BoidDataEvent : public SendableEvent
{
	inline BoidDataEvent(PacketEventId packetId, char xVals[20], char yVals[20], UserId userId = -1, bool isBroadcast = false, UserId receiverId = -1) :
		SendableEvent(EventId::BOID_DATA_EVENT, EventProcessingType::BOTH, isBroadcast, receiverId),
		packetId(packetId), 
		userId(userId) 
	{
		memcpy(posX, xVals, sizeof(char) * 20);
		memcpy(posY, yVals, sizeof(char) * 20);
	}
	virtual ~BoidDataEvent() = default;
	virtual std::size_t allocatePacket(char*& out) override;

	UserId userId;
	PacketEventId packetId;
	char posX[20];
	char posY[20];
};

struct CommandEvent : public SendableEvent
{
	CommandEvent(std::shared_ptr<struct Command> command, bool isBroadcast = false, UserId receiverId = -1);
	virtual ~CommandEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override = 0;

	std::shared_ptr<struct Command> command;
};

struct WhisperCommandEvent : public CommandEvent
{
	WhisperCommandEvent(std::shared_ptr<struct WhisperCommand> command);
	virtual ~WhisperCommandEvent() = default;
	virtual std::size_t allocatePacket(char*& out) override;
};

struct UserDisconnectedEvent : public SendableEvent
{
	UserDisconnectedEvent(UserId userId);

	virtual ~UserDisconnectedEvent() = default;
	virtual void execute() override;
	virtual std::size_t allocatePacket(char*& out) override;

	UserId userId;
};

#endif