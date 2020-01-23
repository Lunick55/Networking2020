#include "ChatRoomScene.h"
#include "SceneManager.h"
#include "ChatRoomServer.h"
#include "ChatRoomClient.h"

void ChatRoomScene::update()
{
	if (ChatRoomClient::isInitialized())
	{
		ChatRoomClient::spInstance->update();
	}
	else if (ChatRoomServer::isInitialized())
	{
		ChatRoomServer::spInstance->update();
	}
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

	if (ChatRoomServer::isInitialized())
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

void ChatRoomScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
		if (ChatRoomClient::isInitialized())
		{
			ChatRoomClient::spInstance->leaveServer();
		}
		else if (ChatRoomServer::isInitialized())
		{
			ChatRoomServer::spInstance->closeChatRoom();
		}

		SceneManager::switchScene(SceneId::MAIN_MENU);
		return;
	}
	else if (input == ENTER_KEY)
	{
		clearInput();
		clearScreenPortion(0, getConsoleCursorY(), getConsoleWidth(), 1);
		setCursorPosition(0, getConsoleCursorY());
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