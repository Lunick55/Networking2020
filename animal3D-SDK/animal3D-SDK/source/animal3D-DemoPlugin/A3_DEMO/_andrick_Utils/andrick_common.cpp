#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

EventSystem& gEventSystem = EventSystem::get();
TextFormatter& gTextFormatter = TextFormatter::get();
a3_DemoState* gDemoState = nullptr;

#ifdef __cplusplus
extern "C"
{
#endif
	// C header here
	void initializeServer()
	{
		gDemoState->mpServer = std::make_shared<Server>();
		gEventSystem.addListener(gDemoState->mpServer, EventProcessingType::SERVERSIDE);
		gDemoState->mpPacketHandler = std::make_shared<PacketHandler>(true);
		std::cout << "Initializing server and server packet handler!" << std::endl;
	}

	void initializeClientPacketHandler()
	{
		//gDemoState->mpClient = std::make_shared<Client>(id, name, authority);
		//gEventSystem.addListener(gDemoState->mpClient, EventProcessingType::CLIENTSIDE);
		gDemoState->mpPacketHandler = std::make_shared<PacketHandler>(false);
		std::cout << "Initializing client packet handler! (Client is still null until it receives a connection accepted packet from server)" << std::endl;
	}
#ifdef __cplusplus
}
#endif