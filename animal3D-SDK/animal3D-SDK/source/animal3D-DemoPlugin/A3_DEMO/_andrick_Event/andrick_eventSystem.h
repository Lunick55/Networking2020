#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <queue>
#include <vector>
#include <functional>
#include <A3_DEMO/_andrick_Event/andrick_event.h>

typedef std::function<void(std::shared_ptr<Event>)> FuncPtr;
typedef std::multimap<EventId, std::shared_ptr<FuncPtr>> MultMap;

class EventListener
{
public:
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) = 0;
};

class EventSystem
{
public:
	EventSystem() {};
	~EventSystem() = default;

	void addListener(EventId eventId, FuncPtr func); // unusable :'(
	void addListener(std::shared_ptr<EventListener> evntListener, EventProcessingType processingType = EventProcessingType::BOTH);

	void removeListener(EventId eventId, FuncPtr func); // unusable :'(
	void removeListener(std::shared_ptr<EventListener> evntListener);
	
	//Two separate functions to streamline event handling so the compiler
	//can optimize the code path without if statements.
	void queueNetworkEvent(std::shared_ptr<SendableEvent> networkEvent);
	void queueLocalEvent(std::shared_ptr<Event> localEvent);

	//Should only be called from a3_demo_callbacks
	void executeQueuedLocalEvents();
	void sendQueuedNetworkEvents();

	static EventSystem& get()
	{
		static EventSystem instance;
		return instance;
	}

private:
	MultMap mListenerFuncMap;
	std::map<std::shared_ptr<EventListener>, EventProcessingType> mEventListeners;

	//Events to process locally. (Maybe they just came from the network 
	//or are simply just events we want to process on the clientside only)
	std::queue<std::shared_ptr<Event>> mQueuedLocalEvents;

	//Events to get sent over the network
	std::queue<std::shared_ptr<SendableEvent>> mQueuedNetworkEvents;
};

//REFERENCE
//void TriggeredFunction(Event* eventPassedIn)
//{
//	EventClassName* eventCLass;
//	eventClass = static_cast<EventClassName*>(eventPassedIn);
//}


#endif