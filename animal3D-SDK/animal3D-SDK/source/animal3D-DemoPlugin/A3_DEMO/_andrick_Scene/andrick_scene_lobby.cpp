#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_lobby.h"
#include "../_andrick_Network/andrick_host.h"

LobbyScene::LobbyScene() :
	Scene(SceneId::Lobby)
{

}

void LobbyScene::input(a3_DemoState* demoState)
{
	//TODO: maybe use a command? like "/play" followed by game
	if (isKeyPressed(demoState, a3key_escape))
	{
		demoState->exitFlag = 1;
		return;
	}
	else if (isKeyPressed(demoState, a3key_T))
	{
		//only if host pressed
		mSelectedGame = GameType::TICTAC;
	}
	else if (isKeyPressed(demoState, a3key_B))
	{
		//only if host pressed
		mSelectedGame = GameType::BATTLESHIP;
	}
}

void LobbyScene::networkReceive(const a3_DemoState* demoState)
{
	//TODO: uhhh. unsure
}

void LobbyScene::update(const a3_DemoState* demoState)
{
	//INIT CHAT ROOM AND GO TO LOBBY SCENE
	if (mSelectedGame == GameType::TICTAC)
	{
		//TODO: GO to the tic tac scene!
		//if (Host::initChatRoom(sPORT, maxUsers, mUsername))
		//{
		//	demoState->mpSceneManager->switchToScene(SceneId::Lobby);
		//}
	}
	else if (mSelectedGame == GameType::BATTLESHIP)
	{
		//TODO: GO to the boat scene!
	}
}

void LobbyScene::render(const a3_DemoState* demoState)
{
	glClearColor(1.0f, 0.5f, 2.0f, 1.0f);

	TextFormatter::get().setLine(0);
	TextFormatter::get().drawText(demoState, "Lobby Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	TextFormatter::get().offsetLine(2);

	if (mSelectedGame == GameType::NONE)
	{
		TextFormatter::get().drawText(demoState, "TIC(T) or Boat(B)?? ;P", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
	}

	//TODO: Find a way to also print chat log?
}


void LobbyScene::networkSend(const a3_DemoState* demoState)
{
	//TODO: send them packets!!!
}