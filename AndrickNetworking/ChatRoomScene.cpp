#include "ChatRoomScene.h"
#include "SceneManager.h"
#include "ChatRoomServer.h"
#include "ChatRoomClient.h"

void ChatRoomScene::update()
{
	if (ChatRoomClient::isHost())
	{
		ChatRoomServer::spInstance->update();
	}

	ChatRoomClient::spInstance->update();
}

void ChatRoomScene::render()
{
}

void ChatRoomScene::drawInitialScene()
{
	std::cout << generateCenteredText("Welcome to the Chat Room Scene!");
	drawLine(1);
	std::cout << std::endl;
	/*std::cout << generateCenteredText("You are now hosting your own chatroom.") << std::endl;*/
}

void ChatRoomScene::enteringScene()
{
	Scene::enteringScene();

	if (ChatRoomClient::isHost())
	{
		//We are the server host.
		ChatRoomServer::spInstance->startChatRoom();
	}
	else if (ChatRoomClient::isInitialized())
	{
		if (ChatRoomClient::spInstance->connectToServer())
		{
			std::cout << "Trying to connect..." << std::endl;
		}
		else
		{
			std::cout << "Failed to connect to server." << std::endl;
		}
	}
	else
	{
		//Uh oh, something went wrong!
	}
}

int numMes = 0;
void ChatRoomScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
		if (ChatRoomClient::isHost())
		{
			ChatRoomServer::spInstance->closeChatRoom();
		}
		else
		{
			ChatRoomClient::spInstance->leaveServer();
		}

		SceneManager::switchScene(SceneId::MAIN_MENU);
		return;
	}
	else if (input == ENTER_KEY)
	{
		clearScreenPortion(0, getConsoleCursorY(), getConsoleWidth(), 1);
		setCursorPosition(0, getConsoleCursorY());

		if (ChatRoomClient::isHost())
		{
			ChatRoomServer::spInstance->sendPublicMessage(ChatRoomServer::spInstance->mpHost, mCurrentInput);
		}
		else
		{
			//TODO: do the same for the client as the server does
			ChatRoomClient::spInstance->sendPublicMessage(mCurrentInput);
		}

		clearInput();
		//setCursorPosition(0, getConsoleCursorY());
	}
	else if (input == BACKSPACE_KEY)
	{
		mCurrentInput.pop_back();
		setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
		std::cout << " ";
		setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
	}
	else
	{
		setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
		mCurrentInput += input;
		std::cout << input;
	}
}