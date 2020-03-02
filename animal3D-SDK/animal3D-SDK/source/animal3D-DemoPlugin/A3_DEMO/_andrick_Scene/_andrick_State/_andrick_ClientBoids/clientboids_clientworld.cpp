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
	mChatHistory(10)
{
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
}

void ClientBoidsClientWorld::processIncomingEvent(std::shared_ptr<Event> evnt)
{

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
	gTextFormatter.drawText("Current mode: Data Push", GREEN, TextAlign::CENTER_X);

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
}
