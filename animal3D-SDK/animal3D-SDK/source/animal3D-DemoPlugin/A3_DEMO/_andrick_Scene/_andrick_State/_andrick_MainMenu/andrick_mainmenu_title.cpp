#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

MainMenuTitle::MainMenuTitle(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU,
		LIGHT_BLUE)
{
	mMenuOptions.push_back(MenuOption(a3key_1, "Press [1] to host a server.", initializeServer, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS));
	mMenuOptions.push_back(MenuOption(a3key_2, "Press [2] to join a server.", initializeClient, SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::JOIN_IP));
	mMenuOptions.push_back(MenuOption(a3key_3, "Press [3] to play locally.", initializeClient, SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::LOCAL_PLAY));
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to exit]", nullptr, SceneId::EXIT, SceneStateId::EXIT_STATE));
}

void MainMenuTitle::enteringState()
{
	SceneState::enteringState();

	if (gDemoState->mIsOnline)
		gEventSystem.queueNetworkEvent(std::make_shared<UserDisconnectedEvent>(gDemoState->mpClient->getId()));
}

void MainMenuTitle::processInput()
{
	SceneState::processInput();
}

void MainMenuTitle::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void MainMenuTitle::update()
{

}

void MainMenuTitle::queueOutgoingEvents()
{

}

void MainMenuTitle::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Welcome to the Main Menu!", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void MainMenuTitle::exitingState()
{
	SceneState::exitingState();
}
