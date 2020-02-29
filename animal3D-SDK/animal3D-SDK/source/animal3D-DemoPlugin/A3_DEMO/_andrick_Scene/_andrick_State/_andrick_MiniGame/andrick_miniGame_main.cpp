#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MiniGame/andrick_miniGame_main.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_miniGame.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

MiniGameMain::MiniGameMain(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)::MiniGameScene::MiniGameSceneStateId::MINIGAME, RED)
{
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press ESC to leave the server.", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void MiniGameMain::enteringState()
{
	a3_DemoState::a3netAddressStr const ipAddress = "127.0.0.1";
	a3ui16 const maxConnections_server = 16;
	a3ui16 const maxConnections_client = 1;

	//if (gDemoState->a3netStartup(0, port_server, 0, maxConnections_client) > 0)
	//{
	//	if (gDemoState->a3netConnect(ipAddress) > 0)
	//	{
	//		printf("\n Client spinning up... \n");
	//	}
	//}

	if (gDemoState->mpPacketHandler->startup(maxConnections_client))
	{
		if (gDemoState->mpPacketHandler->connect(ipAddress))
		{
			std::cout << "Client spinning up..." << std::endl;
		}
		else
		{
			std::cout << "Failed to connect to server." << std::endl;
		}
	}

	//------------------------

	SceneState::enteringState();
}

void MiniGameMain::processInput()
{
	SceneState::processInput();

	//Handle Input
	if (mpInputHandler->isKeyPressed(a3key_space))
	{
		std::shared_ptr<GenericEvent> evnt = nullptr;
		evnt = std::make_shared<GenericEvent>(EventId::INCREMENT_THE_SPACE);
		//gEventSystem.queueLocalEvent(evnt);
		gEventSystem.queueNetworkEvent(evnt);
		//TODO: !!! Make this NetworkEvent
		//or gEventSystem.queueLocalEvent(std::make_shared<Event>(EventId::INCREMENT_THE_SPACE));
		//Dunno which
	}
}

void MiniGameMain::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	SceneState::processIncomingEvent(evnt);
	if (evnt->eventId == EventId::INCREMENT_THE_SPACE)
	{
		IncrementSpace();
	}
}

void MiniGameMain::update()
{
	//main game logic??
}

void MiniGameMain::queueOutgoingEvents() 
{
	//uuuhhhhh	
}

void MiniGameMain::render()
{
	SceneState::render();
	gTextFormatter.setLine(0);
	gTextFormatter.setAlignment(TextAlign::CENTER_X);

	gTextFormatter.drawText("Welcome to Hell!", WHITE);
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText(std::to_string(numberOfTimesSpacePressed), WHITE);
}

void MiniGameMain::exitingState()
{
	SceneState::exitingState();
}

//The function called by event when space pressed.
void MiniGameMain::IncrementSpace()
{
	numberOfTimesSpacePressed++;
}
