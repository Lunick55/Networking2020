#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

//std::shared_ptr<EventSystem> EventSystem::spInstance_ = nullptr;

void EventSystem::AddListener(EventId eventId, FuncPtr func)
{	
	//TODO: maybe check if contained already?
	mListenerFuncMap.insert({ eventId, func });
}

void EventSystem::RemoveListener(EventId eventId, FuncPtr func)
{
	//should get me all the values with the key
	//std::pair<MMAPIterator, MMAPIterator> result = mmapOfPos.equal_range('c');
	std::pair<MultMap::iterator, MultMap::iterator> multKeyResult = mListenerFuncMap.equal_range(eventId);

	MultMap::iterator iter = multKeyResult.first;

	//iterate through those key vlaues, and fire events!
	//for (MMAPIterator it = result.first; it != result.second; it++)
	for (iter; iter != multKeyResult.second; ++iter)
	{
		//TODO: handle this somehow
		//if (iter->second.target == func.target)
		//{
		//	mListenerFuncMap.erase(iter);
		//	return;
		//}
	}
}

void EventSystem::FireEvent(EventId eventId, std::shared_ptr<Event> eventData)
{
	//TODO instead of passing in the event code, just read the eventData's eventID

	//should get me all the values with the key
	//then iterate through those key vlaues, and fire events!

	std::pair<MultMap::iterator, MultMap::iterator> multKeyResult = mListenerFuncMap.equal_range(eventId);
	
	MultMap::iterator iter;
	for (iter = multKeyResult.first; iter != multKeyResult.second; iter++)
	{
		iter->second(eventData);
	}
}