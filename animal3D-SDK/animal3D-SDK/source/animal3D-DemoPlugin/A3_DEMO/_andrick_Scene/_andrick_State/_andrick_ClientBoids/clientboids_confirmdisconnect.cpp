#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_confirmdisconnect.h>
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

ClientBoidsConfirmDisconnect::ClientBoidsConfirmDisconnect(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONFIRM_DISCONNECT, RED)
{
	mMenuOptions.push_back(MenuOption(a3key_enter, "Press [Enter] to disconnect.", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press [ESC] cancel.", nullptr, SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CLIENT_WORLD));
}

void ClientBoidsConfirmDisconnect::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsConfirmDisconnect::processInput()
{
	SceneState::processInput();
}

void ClientBoidsConfirmDisconnect::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ClientBoidsConfirmDisconnect::update()
{

}

void ClientBoidsConfirmDisconnect::queueOutgoingEvents()
{

}

void ClientBoidsConfirmDisconnect::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Are you sure you want to disconnect from the server?", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void ClientBoidsConfirmDisconnect::exitingState()
{
	SceneState::exitingState();
}
