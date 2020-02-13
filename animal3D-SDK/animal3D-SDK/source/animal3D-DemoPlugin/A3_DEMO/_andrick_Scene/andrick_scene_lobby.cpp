#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_lobby.h"
#include "../_andrick_Network/andrick_host.h"

LobbyScene::LobbyScene() :
	Scene(SceneId::Lobby),
	mCurrentStep(LobbyStep::CHATROOM),
	mSelectedGame(GameType::NONE),
	mInitSuccessful(false)
{

}

void LobbyScene::enteringScene(const a3_DemoState* demoState)
{
	Scene::enteringScene(demoState);
	mInitSuccessful = false;

	mCurrentStep = LobbyStep::CHATROOM;

	if (Client::isHost())
	{
		mInitSuccessful = Host::spInstance->startChatRoom(demoState);
	}
	else
	{
		if (Client::spInstance->connectToServer())
		{
			mInitSuccessful = true;
			addToChatList(MessageType::EITHER, "Trying to connect...");
		}
		else
		{
			mInitSuccessful = false;
			addToChatList(MessageType::EITHER, "Failed to connect to server.");
		}
	}
}

//Input handling
void LobbyScene::input(a3_DemoState* demoState)
{
	switch (mCurrentStep)
	{
	case LobbyStep::CHATROOM:
		handleInputChatRoom(demoState);
		break;
	case LobbyStep::LEAVE_SERVER_CONFIRM:
		handleInputLeaveServerConfirm(demoState);
		break;
	default:
		return;
	}
}
void LobbyScene::handleInputChatRoom(a3_DemoState* demoState)
{
	if (isKeyPressed(demoState, a3key_escape))
	{
		mCurrentStep = LobbyStep::LEAVE_SERVER_CONFIRM;
	}
	else if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandResult;
			if (processCommand(demoState, mCurrentInput, command, commandResult))
			{
				if (command.compare(START_GAME_COMMAND) == 0)
				{
					if (commandResult.compare(PLAY_TICTACTOE) == 0)
						mSelectedGame = GameType::TICTAC;
					else
						mSelectedGame = GameType::BATTLESHIP;
				}
			}
			else
			{
				sendPublicMessage(demoState, mCurrentInput, MessageType::EITHER);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void LobbyScene::handleInputLeaveServerConfirm(const a3_DemoState* demoState)
{
	if (isKeyPressed(demoState, a3key_Y))
	{
		mCurrentStep = LobbyStep::LEAVE_SERVER;
	}
	else if (isKeyPressed(demoState, a3key_N))
	{
		mCurrentStep = LobbyStep::CHATROOM;
	}
}

//Incoming packet handling. For now we, propagate the packers to the client/host.
void LobbyScene::networkReceive(const a3_DemoState* demoState)
{
	Scene::networkReceive(demoState);
}

//Update handling.
void LobbyScene::update(const a3_DemoState* demoState)
{
	switch (mCurrentStep)
	{
		case LobbyStep::LEAVE_SERVER:
			handleUpdateLeaveServer(demoState);
			break;
		default:
			return;
	}
}
void LobbyScene::handleUpdateLeaveServer(const a3_DemoState* demoState)
{
	if (Client::isHost()) 
		Host::cleanup();
	else 
		Client::cleanup();

	demoState->mpSceneManager->switchToScene(demoState, SceneId::SelectRole);
}

void LobbyScene::render(const a3_DemoState* demoState)
{
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);

	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText(demoState, "Lobby Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	formatter.offsetLine(2);

	std::vector<LogInfo>::iterator iter = mChatLog.begin();
	for (; iter != mChatLog.end(); ++iter)
		formatter.drawText(demoState, iter->text, iter->color);

	if (mCurrentStep == LobbyStep::LEAVE_SERVER_CONFIRM)
		formatter.drawText(demoState, "Are you sure you want to leave the server? (Y/N)");

	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput);
}


void LobbyScene::networkSend(const a3_DemoState* demoState)
{
	switch (mCurrentStep)
	{
	case LobbyStep::CHATROOM:
		handleNetworkSendChatRoomStep(demoState);
		break;
	default:
		return;
	}
}



void LobbyScene::handleNetworkSendChatRoomStep(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		switch (mSelectedGame)
		{
		case GameType::TICTAC:
			Host::spInstance->serverTravel(demoState, SceneId::Tictactoe);
			break;
		case GameType::BATTLESHIP:
			Host::spInstance->serverTravel(demoState, SceneId::Battleship);
			break;
		default:
			return;
		}
	}
}