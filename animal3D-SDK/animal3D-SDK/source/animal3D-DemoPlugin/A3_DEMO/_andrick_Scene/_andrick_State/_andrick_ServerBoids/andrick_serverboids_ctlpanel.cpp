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
	mChatHistory(10),
	mDataMode(andrick_ID_GENERIC_DATA_PUSH_EVENT),
	mDataModeText("Data Push")
{
	mDataModeMap.insert({ andrick_ID_GENERIC_DATA_PUSH_EVENT, "Data Push" });
	mDataModeMap.insert({ andrick_ID_GENERIC_DATA_SHARE_EVENT, "Data Share" });
	mDataModeMap.insert({ andrick_ID_GENERIC_DATA_COUPLE_EVENT, "Data Couple" });

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

	if (mpInputHandler->isKeyPressed(a3key_1))
	{
		std::shared_ptr<GenericEvent> dataPushEvnt = std::make_shared<GenericEvent>(PacketEventId::andrick_ID_GENERIC_DATA_PUSH_EVENT);
		gEventSystem.queueLocalEvent(dataPushEvnt);
		gEventSystem.queueNetworkEvent(dataPushEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_2))
	{
		std::shared_ptr<GenericEvent> dataShareEvnt = std::make_shared<GenericEvent>(PacketEventId::andrick_ID_GENERIC_DATA_SHARE_EVENT);
		gEventSystem.queueLocalEvent(dataShareEvnt);
		gEventSystem.queueNetworkEvent(dataShareEvnt);
	}
	else if (mpInputHandler->isKeyPressed(a3key_3))
	{
		std::shared_ptr<GenericEvent> dataCoupleEvnt = std::make_shared<GenericEvent>(PacketEventId::andrick_ID_GENERIC_DATA_COUPLE_EVENT);
		gEventSystem.queueLocalEvent(dataCoupleEvnt);
		gEventSystem.queueNetworkEvent(dataCoupleEvnt);
	}

	mpInputHandler->clearCurrentInput();

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
	switch (evnt->eventId)
	{
	case EventId::GENERIC_EVENT:
	{
		std::shared_ptr<GenericEvent> genericEvnt = std::dynamic_pointer_cast<GenericEvent>(evnt);
		handleGenericEvents(genericEvnt);
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
		WHITE, TextAlign::RIGHT);

	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::RIGHT, 1);
}

void ServerBoidsControlPanel::exitingState()
{
	SceneState::exitingState();

	mDataMode = andrick_ID_GENERIC_DATA_PUSH_EVENT;
	mDataModeText = "Data Push";
}

void ServerBoidsControlPanel::handleGenericEvents(std::shared_ptr<GenericEvent> genericEvnt)
{
	switch (genericEvnt->packetId)
	{
	case PacketEventId::andrick_ID_GENERIC_DATA_PUSH_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_GENERIC_DATA_PUSH_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_GENERIC_DATA_PUSH_EVENT)->second;
		break;
	}
	case PacketEventId::andrick_ID_GENERIC_DATA_SHARE_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_GENERIC_DATA_SHARE_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_GENERIC_DATA_SHARE_EVENT)->second;
		break;
	}
	case PacketEventId::andrick_ID_GENERIC_DATA_COUPLE_EVENT:
	{
		mDataMode = mDataModeMap.find(andrick_ID_GENERIC_DATA_COUPLE_EVENT)->first;
		mDataModeText = mDataModeMap.find(andrick_ID_GENERIC_DATA_COUPLE_EVENT)->second;
		break;
	}
	default:
		break;
	}
}