#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ServerBoids/andrick_serverboids_ctlpanel.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_serverboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

ServerBoidsControlPanel::ServerBoidsControlPanel(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::CONTROL_PANEL, DARK_GREY),
	mChatHistory(10)
{
	mMenuOptions.push_back(MenuOption(a3key_1, "Press [1] for Data Push."));
	mMenuOptions.push_back(MenuOption(a3key_2, "Press [2] for Data Sharing."));
	mMenuOptions.push_back(MenuOption(a3key_3, "Press [3] for Data Couple."));
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to stop server]", nullptr, SceneId::SERVER_BOIDS, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::SHUTDOWN_SERVER_CONFIRMATION));
}

void ServerBoidsControlPanel::enteringState()
{
	if (gDemoState->mpPacketHandler->startup(gDemoState->mpServer->getMaxUserCount()))
	{
		printf("\nServer spinning up... \n");
	}

	SceneState::enteringState();
}

void ServerBoidsControlPanel::processInput()
{
	SceneState::processInput();

	//if (mpInputHandler->isKeyPressed(a3key_enter))
	//{
	//	//Check if the input is a command. If it is, fire the respective command event.
	//	if (!mpInputHandler->handleCommandInput())
	//	{
	//		//Send event a public message event.
	//		mpInputHandler->getChatLog()->append(MessageType::GLOBAL, mpInputHandler->getCurrentInput());
	//
	//		//TODO: Don't add to log here in the future.
	//		mChatLogHistory = mpInputHandler->getChatLog()->getRecentChatLog(0, mChatHistory);
	//
	//		//TODO: Fire public message event.
	//		//gEventSystem.QueueEvent(std::shared_ptr<PublicMessageEvent>(gDemoState->mpCurrentUser, mpInputHandler->getCurrentInput()));
	//	}
	//
	//	mpInputHandler->clearCurrentInput();
	//}
}

void ServerBoidsControlPanel::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ServerBoidsControlPanel::update()
{

}

void ServerBoidsControlPanel::queueOutgoingEvents()
{

}

void ServerBoidsControlPanel::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Welcome to your server!", WHITE, TextAlign::CENTER_X);
	
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText("Current mode: Data Push", GREEN, TextAlign::CENTER_X);
	
	gTextFormatter.setLine(9);
	renderMenuOptions(WHITE, TextAlign::LEFT);
	
	gTextFormatter.setLine(7);
	gTextFormatter.drawText(
		std::to_string(gDemoState->mpServer->getConnectedUserCount()) + "/" + 
		std::to_string(gDemoState->mpServer->getMaxUserCount()) + " Clients Online", 
		WHITE, TextAlign::RIGHT);

	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::RIGHT, 1);
}

void ServerBoidsControlPanel::exitingState()
{
	SceneState::exitingState();
}
