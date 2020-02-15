#include <A3_DEMO/_andrick_Scene/andrick_scene_lobby.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
//#include "../_andrick_Network/andrick_host.h"

LobbyScene::LobbyScene() :
	Scene(SceneId::Lobby),
	mCurrentStep(LobbyStep::CHATROOM),
	mSelectedGame(GameType::NONE),
	mInitSuccessful(false)
{

}

void LobbyScene::enteringScene()
{
	Scene::enteringScene();
	mInitSuccessful = false;

	mCurrentStep = LobbyStep::CHATROOM;

	//if (Client::isHost())
	//{
	//	mInitSuccessful = Host::spInstance->startChatRoom(demoState);
	//}
	//else
	//{
	//	if (Client::spInstance->connectToServer())
	//	{
	//		mInitSuccessful = true;
	//		addToChatList(MessageType::EITHER, "Trying to connect...");
	//	}
	//	else
	//	{
	//		mInitSuccessful = false;
	//		addToChatList(MessageType::EITHER, "Failed to connect to server.");
	//	}
	//}
}

//Input handling
void LobbyScene::input()
{
	switch (mCurrentStep)
	{
	case LobbyStep::CHATROOM:
		handleInputChatRoom();
		break;
	case LobbyStep::LEAVE_SERVER_CONFIRM:
		handleInputLeaveServerConfirm();
		break;
	default:
		return;
	}
}
void LobbyScene::handleInputChatRoom()
{
	if (mpInputHandler->isKeyPressed(a3key_escape))
	{
		mCurrentStep = LobbyStep::LEAVE_SERVER_CONFIRM;
	}
	else if (mpInputHandler->isKeyPressed(a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandResult;
			if (processCommand(mCurrentInput, command, commandResult))
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
				sendPublicMessage(mCurrentInput, MessageType::EITHER);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input();
	}
}
void LobbyScene::handleInputLeaveServerConfirm()
{
	if (mpInputHandler->isKeyPressed(a3key_Y))
	{
		mCurrentStep = LobbyStep::LEAVE_SERVER;
	}
	else if (mpInputHandler->isKeyPressed(a3key_N))
	{
		mCurrentStep = LobbyStep::CHATROOM;
	}
}

//Incoming packet handling. For now we, propagate the packers to the client/host.
void LobbyScene::processIncomingEvents()
{
	Scene::processIncomingEvents();
}

//Update handling.
void LobbyScene::update()
{
	switch (mCurrentStep)
	{
		case LobbyStep::LEAVE_SERVER:
			handleUpdateLeaveServer();
			break;
		default:
			return;
	}
}
void LobbyScene::handleUpdateLeaveServer()
{
	//if (Client::isHost()) 
	//	Host::cleanup();
	//else 
	//	Client::cleanup();

	gDemoState->mpSceneManager->switchToScene(SceneId::SelectRole);
}

void LobbyScene::render()
{
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);

	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText("Lobby Scene", AndrickColors::WHITE, TextAlign::CENTER_X);
	formatter.offsetLine(2);

	std::vector<LogInfo>::iterator iter = mChatLog.begin();
	for (; iter != mChatLog.end(); ++iter)
		formatter.drawText(iter->text, iter->color);

	if (mCurrentStep == LobbyStep::LEAVE_SERVER_CONFIRM)
		formatter.drawText("Are you sure you want to leave the server? (Y/N)");

	formatter.offsetLine(2);
	formatter.drawText(mCurrentInput);
}


void LobbyScene::packageOutgoingEvents()
{
	switch (mCurrentStep)
	{
	case LobbyStep::CHATROOM:
		handleNetworkSendChatRoomStep();
		break;
	default:
		return;
	}
}



void LobbyScene::handleNetworkSendChatRoomStep()
{
	//if (Client::isHost())
	//{
	//	switch (mSelectedGame)
	//	{
	//	case GameType::TICTAC:
	//		Host::spInstance->serverTravel(demoState, SceneId::Tictactoe);
	//		break;
	//	case GameType::BATTLESHIP:
	//		Host::spInstance->serverTravel(demoState, SceneId::Battleship);
	//		break;
	//	default:
	//		return;
	//	}
	//}
}