#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

#include <chrono>

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

	void initializeClient()
	{
		gDemoState->mpClient = std::make_shared<Client>();
		gEventSystem.addListener(gDemoState->mpClient, EventProcessingType::CLIENTSIDE);
		gDemoState->mpPacketHandler = std::make_shared<PacketHandler>(false);
		std::cout << "Initializing client packet handler!" << std::endl;
	}

	void shutdownRakNet()
	{
		if (gDemoState->mpServer)
		{
			gEventSystem.removeListener(gDemoState->mpServer);
			gDemoState->mpServer.reset();
		}

		if (gDemoState->mpClient)
		{
			gEventSystem.removeListener(gDemoState->mpClient);
			gDemoState->mpClient.reset();
		}

		if (gDemoState->mpPacketHandler)
		{
			if (gDemoState->mpPacketHandler->disconnect())
			{
				std::cout << "Successfully disconnected." << std::endl;
				if (gDemoState->mpPacketHandler->shutdown())
				{
					std::cout << "Successfully shutdown RakNet." << std::endl;
				}
			}
		}
	}

	RakNet::Time getTime()
	{
		using namespace std::chrono;
		milliseconds ms = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
			);
		//std::printf("Time: %I64i\n", ms.count());

		return ms.count();
	}

#ifdef __cplusplus
}
#endif