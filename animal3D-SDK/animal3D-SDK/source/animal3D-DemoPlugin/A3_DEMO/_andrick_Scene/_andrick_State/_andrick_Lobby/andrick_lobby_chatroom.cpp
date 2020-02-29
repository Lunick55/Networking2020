#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_Lobby/andrick_lobby_chatroom.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_lobby.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

LobbyChatroom::LobbyChatroom(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)LobbyScene::LobbySceneStateId::CHATROOM, DARK_GREY),
	mChatHistory(10)
{
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press ESC to leave the server.", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void LobbyChatroom::enteringState()
{
	SceneState::enteringState();
}

void LobbyChatroom::processInput()
{
	SceneState::processInput();

	if (mpInputHandler->isKeyPressed(a3key_enter))
	{
		//Check if the input is a command. If it is, fire the respective command event.
		if (!mpInputHandler->handleCommandInput())
		{
			//Send event a public message event.
			mpInputHandler->getChatLog()->append(MessageType::GLOBAL, mpInputHandler->getCurrentInput());
			
			//TODO: Don't add to log here in the future.
			mChatLogHistory = mpInputHandler->getChatLog()->getRecentChatLog(0, mChatHistory);
			
			//TODO: Fire public message event.
			//gEventSystem.QueueEvent(std::shared_ptr<PublicMessageEvent>(gDemoState->mpCurrentUser, mpInputHandler->getCurrentInput()));
		}

		mpInputHandler->clearCurrentInput();
	}
}

void LobbyChatroom::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	SceneState::processIncomingEvent(evnt);
}

void LobbyChatroom::update()
{

}

void LobbyChatroom::queueOutgoingEvents()
{

}

void LobbyChatroom::render()
{
	SceneState::render();
	gTextFormatter.setLine(0);
	gTextFormatter.setAlignment(TextAlign::CENTER_X);

	gTextFormatter.drawText("Lobby Chatroom", WHITE);
	gTextFormatter.offsetLine(6);
	renderMenuOptions(WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::LEFT, 1);
	gTextFormatter.setLine(20);
	gTextFormatter.drawText(mpInputHandler->getCurrentInput(), WHITE);
}

void LobbyChatroom::exitingState()
{
	SceneState::exitingState();
}