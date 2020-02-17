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

}