#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_localplay.h>
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


ClientBoidsLocalPlay::ClientBoidsLocalPlay(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::LOCAL_PLAY, DARK_GREY),
	mDataMode(andrick_ID_BOID_DATA_PUSH_EVENT),
	mDataModeText("Data Push")
{
	mDataModeMap.insert({ andrick_ID_BOID_DATA_PUSH_EVENT, "Data Push" });
	mDataModeMap.insert({ andrick_ID_BOID_DATA_SHARE_EVENT, "Data Share" });
	mDataModeMap.insert({ andrick_ID_BOID_DATA_COUPLE_EVENT, "Data Couple" });

	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to main menu]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void ClientBoidsLocalPlay::enteringState()
{
	for (int i = 0; i < 20; i++)
	{
		Boid* pBoid = gDemoState->mpBoidManager->createRandomUnit();
		if (pBoid == NULL)
		{
			gDemoState->mpBoidManager->deleteRandomUnit();
		}
	}

	SceneState::enteringState();
}

void ClientBoidsLocalPlay::processInput()
{
	SceneState::processInput();

	float temp[20];

	if (mpInputHandler->isKeyPressed(a3key_1))
	{
		std::shared_ptr<BoidDataEvent> dataPushEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_PUSH_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataPushEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_2))
	{
		std::shared_ptr<BoidDataEvent> dataShareEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_SHARE_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataShareEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_3))
	{
		std::shared_ptr<BoidDataEvent> dataCoupleEvnt = std::make_shared<BoidDataEvent>(PacketEventId::andrick_ID_BOID_DATA_COUPLE_EVENT, temp, temp);
		gEventSystem.queueLocalEvent(dataCoupleEvnt);
	}

	mpInputHandler->clearCurrentInput();
}

void ClientBoidsLocalPlay::processIncomingEvent(std::shared_ptr<Event> evnt)
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

void ClientBoidsLocalPlay::update()
{
	gDemoState->mpBoidManager->updateAll((float)gDemoState->renderTimer->secondsPerTick);
}

void ClientBoidsLocalPlay::queueOutgoingEvents()
{

}

void ClientBoidsLocalPlay::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Client boids local play.", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText("Current mode:" + mDataModeText, GREEN, TextAlign::LEFT);
	gTextFormatter.offsetLine(6);

	//draw units
	gDemoState->mpBoidManager->drawAll();

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void ClientBoidsLocalPlay::exitingState()
{
	for (int i = 0; i < 20; i++)
	{
		gDemoState->mpBoidManager->deleteRandomUnit();
	}

	SceneState::exitingState();

	mDataMode = andrick_ID_BOID_DATA_PUSH_EVENT;
	mDataModeText = "Data Push";
}


void ClientBoidsLocalPlay::handleBoidDataEvents(std::shared_ptr<BoidDataEvent> boidEvnt)
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