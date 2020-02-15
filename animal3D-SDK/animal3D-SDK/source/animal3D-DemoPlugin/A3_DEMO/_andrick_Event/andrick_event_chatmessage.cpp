#include <A3_DEMO/_andrick_Event/andrick_event_chatmessage.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>

ChatMessageEvent::ChatMessageEvent(std::shared_ptr<Client> sender, std::shared_ptr<Client> reciever,
	const std::string& message) :
	Event(EventId::CHAT_MESSAGE),
	mpSender(sender),
	mpReciever(reciever),
	mMessage(message)
{

}

void ChatMessageEvent::execute()
{
	//std::shared_ptr<Event> messageEvent;

	//Send event to display public chat message
	if (!mpReciever)
	{
		//messageEvent = std::shared_ptr<PublicChatEvent>(mSender. mMessage);
	}
	else
	{
		//messageEvent = std::shared_ptr<PrivateChatEvent>(mSender, mReciever, mMessage);
	}

	//gDemoState->mpEventManager->fireEvent(messageEvent);
}