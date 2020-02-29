#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ServerBoids/andrick_serverboids_shutdown.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_serverboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

ServerBoidsShutdownConfirmation::ServerBoidsShutdownConfirmation(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::SHUTDOWN_SERVER_CONFIRMATION, RED)
{
	mMenuOptions.push_back(MenuOption(a3key_enter, "Press [ENTER] to shutdown.", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press [ESC] to return to server control panel.", nullptr, SceneId::SERVER_BOIDS, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::CONTROL_PANEL));
}

void ServerBoidsShutdownConfirmation::enteringState()
{
	SceneState::enteringState();
}

void ServerBoidsShutdownConfirmation::processInput()
{
	SceneState::processInput();
}

void ServerBoidsShutdownConfirmation::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ServerBoidsShutdownConfirmation::update()
{

}

void ServerBoidsShutdownConfirmation::queueOutgoingEvents()
{

}

void ServerBoidsShutdownConfirmation::render()
{
	SceneState::render();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Are you sure you want to shut down the server?", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void ServerBoidsShutdownConfirmation::exitingState()
{
	SceneState::exitingState();
}
