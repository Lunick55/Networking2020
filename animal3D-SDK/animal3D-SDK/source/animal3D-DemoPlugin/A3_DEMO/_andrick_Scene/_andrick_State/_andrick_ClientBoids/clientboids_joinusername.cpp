#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_joinusername.h>
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

ClientBoidsJoinUsername::ClientBoidsJoinUsername(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::JOIN_USERNAME, DARK_GREY)
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to main menu]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void ClientBoidsJoinUsername::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsJoinUsername::processInput()
{
	SceneState::processInput();
}

void ClientBoidsJoinUsername::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ClientBoidsJoinUsername::update()
{

}

void ClientBoidsJoinUsername::queueOutgoingEvents()
{

}

void ClientBoidsJoinUsername::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Client boids join username.", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText("Current mode: Data Push", GREEN, TextAlign::LEFT);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void ClientBoidsJoinUsername::exitingState()
{
	SceneState::exitingState();
}
