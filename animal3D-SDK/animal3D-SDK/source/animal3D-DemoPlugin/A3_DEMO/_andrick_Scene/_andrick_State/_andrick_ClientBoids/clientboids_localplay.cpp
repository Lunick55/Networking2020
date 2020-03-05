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
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::LOCAL_PLAY, DARK_GREY)
{
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
}

void ClientBoidsLocalPlay::processIncomingEvent(std::shared_ptr<Event> evnt)
{

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
	gTextFormatter.drawText("Current mode: Data Push", GREEN, TextAlign::LEFT);
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
}
