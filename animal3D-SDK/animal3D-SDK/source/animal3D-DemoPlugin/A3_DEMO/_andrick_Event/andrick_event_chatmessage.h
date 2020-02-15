#ifndef CHATMESSAGE_EVENT_H_
#define CHATMESSAGE_EVENT_H_

#include <A3_DEMO/_andrick_Event/andrick_event.h>

struct ChatMessageEvent : public Event
{
	ChatMessageEvent(class std::shared_ptr<class Client> sender, class std::shared_ptr<class Client> reciever,
		const std::string& message);
	virtual ~ChatMessageEvent() = default;

	virtual void execute() override;

	std::shared_ptr<class Client> mpSender;
	std::shared_ptr<class Client> mpReciever;
	std::string mMessage;
};

#endif