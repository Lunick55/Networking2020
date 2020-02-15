#ifndef EVENT_H_
#define EVENT_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

struct Event
{
public:
	Event(EventId id) : ID(id) {}
	virtual ~Event();

	virtual void execute() = 0;

	const EventId ID;
};

#endif