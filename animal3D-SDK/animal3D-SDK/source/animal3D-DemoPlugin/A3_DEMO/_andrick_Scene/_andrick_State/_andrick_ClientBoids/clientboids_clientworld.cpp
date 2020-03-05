#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_clientworld.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>

ClientBoidsClientWorld::ClientBoidsClientWorld(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CLIENT_WORLD, LIGHT_BLUE),
	mChatHistory(10),
	mDataMode(andrick_ID_BOID_DATA_PUSH_EVENT),
	mDataModeText("Data Push")
{
	mDataModeMap.insert({ andrick_ID_BOID_DATA_PUSH_EVENT, "Data Push" });
	mDataModeMap.insert({ andrick_ID_BOID_DATA_SHARE_EVENT, "Data Share" });
	mDataModeMap.insert({ andrick_ID_BOID_DATA_COUPLE_EVENT, "Data Couple" });

	mMenuOptions.push_back(MenuOption(a3key_1, "Press [1] for Data Push."));
	mMenuOptions.push_back(MenuOption(a3key_2, "Press [2] for Data Sharing."));
	mMenuOptions.push_back(MenuOption(a3key_3, "Press [3] for Data Couple."));
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to disconnect]", nullptr, SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONFIRM_DISCONNECT));
}

void ClientBoidsClientWorld::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsClientWorld::processInput()
{
	SceneState::processInput();

	char temp[20];

	if (mpInputHandler->isKeyPressed(a3key_1))
	{
		std::shared_ptr<BoidDataEvent> dataPushEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_PUSH_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataPushEvnt);
		gEventSystem.queueNetworkEvent(dataPushEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_2))
	{
		std::shared_ptr<BoidDataEvent> dataShareEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_SHARE_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataShareEvnt);
		gEventSystem.queueNetworkEvent(dataShareEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_3))
	{
		std::shared_ptr<BoidDataEvent> dataCoupleEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_COUPLE_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataCoupleEvnt);
		gEventSystem.queueNetworkEvent(dataCoupleEvnt);
	}

	mpInputHandler->clearCurrentInput();
}

void ClientBoidsClientWorld::processIncomingEvent(std::shared_ptr<Event> evnt)
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

void ClientBoidsClientWorld::update()
{

}

void ClientBoidsClientWorld::queueOutgoingEvents()
{

}

void ClientBoidsClientWorld::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Welcome to the Boids Server!", WHITE, TextAlign::CENTER_X);

	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText("Current mode: " + mDataModeText, GREEN, TextAlign::CENTER_X);

	gTextFormatter.setLine(5);
	renderMenuOptions(WHITE, TextAlign::LEFT);

	gTextFormatter.setLine(4);
	gTextFormatter.drawText(
		std::to_string(gDemoState->mpClient->getConnectedUserCount()) + "/" +
		std::to_string(gDemoState->mpClient->getMaxUserCount()) + " Clients Online",
		WHITE, TextAlign::RIGHT);

	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::RIGHT, 1);
}

void ClientBoidsClientWorld::exitingState()
{
	SceneState::exitingState();

	mDataMode = andrick_ID_BOID_DATA_PUSH_EVENT;
	mDataModeText = "Data Push";
}

void ClientBoidsClientWorld::handleBoidDataEvents(std::shared_ptr<BoidDataEvent> boidEvnt)
{
	switch (boidEvnt->packetId)
	{
	case PacketEventId::andrick_ID_BOID_DATA_PUSH_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_BOID_DATA_PUSH_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_BOID_DATA_PUSH_EVENT)->second;
		break;
	}
	case PacketEventId::andrick_ID_BOID_DATA_SHARE_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_BOID_DATA_SHARE_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_BOID_DATA_SHARE_EVENT)->second;
		break;
	}
	case PacketEventId::andrick_ID_BOID_DATA_COUPLE_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_BOID_DATA_COUPLE_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_BOID_DATA_COUPLE_EVENT)->second;
		break;
	}
	default:
		break;
	}
}