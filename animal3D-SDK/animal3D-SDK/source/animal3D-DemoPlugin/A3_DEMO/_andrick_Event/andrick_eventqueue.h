#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <queue>

class EventQueue
{
public:
	
private:
	std::queue<std::shared_ptr<Event>> mpQueuedEvents;
};

#endif