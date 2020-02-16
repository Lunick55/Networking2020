#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <string>
#include <functional>
#include "andrick_event.h"

typedef std::function<void(std::shared_ptr<Event>)> FuncPtr;
typedef std::multimap < std::string, FuncPtr> MultMap;

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

	void AddListener(std::string eventName, FuncPtr func);
	void RemoveListener(std::string eventName, FuncPtr func);
	void FireEvent(std::string eventName, std::shared_ptr<Event> eventData);

private:
	//static std::shared_ptr<EventSystem> spInstance_;

	MultMap mListenerFuncMap;
};

//REFERENCE
//void TriggeredFunction(Event* eventPassedIn)
//{
//	EventClassName* eventCLass;
//	eventClass = static_cast<EventClassName*>(eventPassedIn);
//}


#endif