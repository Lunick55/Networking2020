#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <string>
#include <functional>
#include <A3_DEMO/_andrick_Event/andrick_event.h>

typedef std::function<void(std::shared_ptr<Event>)> FuncPtr;
typedef std::multimap<EventId, FuncPtr> MultMap;

class EventSystem
{
public:
	//static std::shared_ptr<EventSystem> spInstance()
	//{
	//	//static EventSystem* instance = new EventSystem();
	//	if (!spInstance_)
	//	{
	//		spInstance_ = std::make_shared<EventSystem>();
	//	}
	//	return spInstance_;
	//}
	EventSystem() {};
	~EventSystem() = default;

	void AddListener(EventId eventId, FuncPtr func);
	void RemoveListener(EventId eventId, FuncPtr func);
	void FireEvent(EventId eventId, std::shared_ptr<Event> eventData);

	static EventSystem& get()
	{
		static EventSystem instance;
		return instance;
	}

private:
	MultMap mListenerFuncMap;
};

//REFERENCE
//void TriggeredFunction(Event* eventPassedIn)
//{
//	EventClassName* eventCLass;
//	eventClass = static_cast<EventClassName*>(eventPassedIn);
//}


#endif