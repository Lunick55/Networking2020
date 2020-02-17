#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

void EventSystem::addListener(EventId eventId, FuncPtr func)
{	
	std::shared_ptr<FuncPtr> funcWrapper = std::make_shared<FuncPtr>(func);
	//TODO: maybe check if contained already?
	mListenerFuncMap.insert({ eventId, funcWrapper });
}

void EventSystem::addListener(std::shared_ptr<EventAgnosticListener> evntListener)
{
	std::vector< std::shared_ptr<EventAgnosticListener>>::iterator iter = mEventAgnosticListeners.begin();

	for (; iter != mEventAgnosticListeners.end(); ++iter)
	{
		if (*iter == evntListener)
		{
			return;
		}
	}

	mEventAgnosticListeners.push_back(evntListener);
}

void EventSystem::removeListener(EventId eventId, FuncPtr func)
{
	std::shared_ptr<FuncPtr> funcWrapper = std::make_shared<FuncPtr>(func);
	//should get me all the values with the key
	//std::pair<MMAPIterator, MMAPIterator> result = mmapOfPos.equal_range('c');
	std::pair<MultMap::iterator, MultMap::iterator> multKeyResult = mListenerFuncMap.equal_range(eventId);

	MultMap::iterator iter = multKeyResult.first;

	//iterate through those key vlaues, and fire events!
	//for (MMAPIterator it = result.first; it != result.second; it++)
	for (; iter != multKeyResult.second; ++iter)
	{
		//TODO: handle this somehow
		//std::functions are incomparable, must wrap them in a shared_ptr to compare
		if (iter->second == funcWrapper)
		{
			mListenerFuncMap.erase(iter);
			return;
		}
	}
}

void EventSystem::removeListener(std::shared_ptr<EventAgnosticListener> evntListener)
{
	std::vector<std::shared_ptr<EventAgnosticListener>>::iterator iter = mEventAgnosticListeners.begin();

	for (; iter != mEventAgnosticListeners.end(); ++iter)
	{
		if (*iter == evntListener)
		{
			mEventAgnosticListeners.erase(iter);
			return;
		}
	}
}

void EventSystem::queueLocalEvent(std::shared_ptr<Event> eventData)
{
	eventData->dispatchType = EventDispatchType::LOCAL;
	mQueuedLocalEvents.push(eventData);
}

void EventSystem::queueNetworkEvent(std::shared_ptr<Event> eventData)
{
	eventData->dispatchType = EventDispatchType::NETWORK;
	mQueuedNetworkEvents.push(eventData);
}

//Executes all of the queued local events.
//Events coming in from over the network get
//queued up in here as well as general local 
//events from the client
void EventSystem::executeQueuedLocalEvents()
{
	while (!mQueuedLocalEvents.empty())
	{
		std::shared_ptr<Event> eventData = mQueuedLocalEvents.front();
		mQueuedLocalEvents.pop();

		//should get me all the values with the key
		//then iterate through those key vlaues, and fire events!
		std::pair<MultMap::iterator, MultMap::iterator> multKeyResult = mListenerFuncMap.equal_range(eventData->ID);

		MultMap::iterator multMapIter;
		for (multMapIter = multKeyResult.first; multMapIter != multKeyResult.second; multMapIter++)
		{
			(*multMapIter->second)(eventData);
		}

		std::vector<std::shared_ptr<EventAgnosticListener>>::iterator agnosticListenersIter = mEventAgnosticListeners.begin();
		for (; agnosticListenersIter != mEventAgnosticListeners.end(); ++agnosticListenersIter)
		{
			(*agnosticListenersIter)->processIncomingEvent(eventData);
		}
	}
}

//Sends off all of the queued up network events over the server
//TODO: Server RakNet packet loop to check for incoming packets,
//convert them back into events, and queue them up as local events
//on the other client
void EventSystem::sendQueuedNetworkEvents()
{
	while (!mQueuedNetworkEvents.empty())
	{
		std::shared_ptr<Event> eventData = mQueuedNetworkEvents.front();
		mQueuedNetworkEvents.pop();

		const char* packet;
		PacketEventId packetId = eventData->generatePacket(packet);
		std::cout << "Sending of packet with ID: " << std::to_string((int)packetId) << "!" << std::endl;
		//TODO: send off packet over the network
	}
}