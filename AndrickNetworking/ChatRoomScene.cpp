#include "Scene.h"
#include "SceneManager.h"

void ChatRoomScene::update()
{
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

void ChatRoomScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
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