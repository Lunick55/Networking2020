#ifndef TEST_EVENT_H_
#define TEST_EVENT_H_

#include <A3_DEMO/_andrick_Event/andrick_event.h>

struct TestEvent : public Event
{
	TestEvent(const std::string& message, int num) : Event(EventId::CHAT_MESSAGE), mMessage(message), number(num) { };
	virtual ~TestEvent() = default;

	virtual void execute() override {};

	int number;
	std::string mMessage;
};

#endif
