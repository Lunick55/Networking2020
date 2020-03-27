#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ServerBoids/andrick_serverboids_ctlpanel.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_serverboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

ServerBoidsControlPanel::ServerBoidsControlPanel(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::CONTROL_PANEL, DARK_GREY),
	mChatHistory(10),
	mDataMode(andrick_ID_BOID_DATA_PUSH_EVENT),
	mDataModeText("Data Push")
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to stop server]", nullptr, SceneId::SERVER_BOIDS, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::SHUTDOWN_SERVER_CONFIRMATION));
}

void ServerBoidsControlPanel::enteringState()
{
	if (gDemoState->mpPacketHandler->startup(gDemoState->mpServer->getMaxUserCount()))
	{
		printf("\nServer spinning up... \n");
	}

	//for (int i = 0; i < BOID_COUNT; i++)
	//{
	//	Boid* pBoid = gDemoState->mpBoidManager->createRandomUnit();
	//	if (pBoid == NULL)
	//	{
	//		gDemoState->mpBoidManager->deleteRandomUnit();
	//	}
	//}

	SceneState::enteringState();
}

void ServerBoidsControlPanel::processInput()
{
	SceneState::processInput();
	mpInputHandler->clearCurrentInput();
}

void ServerBoidsControlPanel::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::BOID_DATA_EVENT:
	{
		std::shared_ptr<BoidDataEvent> boidEvnt = std::dynamic_pointer_cast<BoidDataEvent>(evnt);
		handleBoidDataEvents(boidEvnt);
		break;
	}
	default:
		break;
	}
}

void ServerBoidsControlPanel::update()
{
}

void ServerBoidsControlPanel::queueOutgoingEvents()
{
	//send all incoming events over the network
}

void ServerBoidsControlPanel::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Welcome to your server!", WHITE, TextAlign::CENTER_X);
	
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText("Current mode: " + mDataModeText, GREEN, TextAlign::CENTER_X);
	
	gTextFormatter.setLine(9);
	renderMenuOptions(WHITE, TextAlign::LEFT);
	
	gTextFormatter.setLine(7);
	gTextFormatter.drawText(
		std::to_string(gDemoState->mpServer->getConnectedUserCount()) + "/" + 
		std::to_string(gDemoState->mpServer->getMaxUserCount()) + " Clients Online", 
		WHITE, TextAlign::RIGHT
	);

	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::RIGHT, 1);
}

void ServerBoidsControlPanel::exitingState()
{
	SceneState::exitingState();

	//for (int i = 0; i < BOID_COUNT; i++)
	//{
	//	gDemoState->mpBoidManager->deleteRandomUnit();
	//}

	mDataMode = andrick_ID_BOID_DATA_PUSH_EVENT;
	mDataModeText = "Data Push";
}

void ServerBoidsControlPanel::handleBoidDataEvents(std::shared_ptr<BoidDataEvent> boidEvnt)
{
	//TODO: Send the boid event to everyone else except for the user that sent it.
	gEventSystem.queueNetworkEvent(boidEvnt);
}