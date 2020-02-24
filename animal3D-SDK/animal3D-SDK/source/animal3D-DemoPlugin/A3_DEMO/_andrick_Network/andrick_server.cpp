#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>

UserId Server::sUserIdCounter = 0;

Server::Server()
{

}

void Server::processIncomingEvent(std::shared_ptr<struct Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::CONNECTION_REQUEST_ACCEPTED:
		std::cout << "Connection was accepted!" << std::endl;
		//Create join accepted event to send over network
		break;
	default:
		break;
	}
}