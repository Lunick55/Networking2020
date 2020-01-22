#include "Scene.h"
#include "SceneManager.h"

void CreateChatRoomScene::update()
{
}

void CreateChatRoomScene::render()
{
}

void CreateChatRoomScene::drawInitialScene()
{
	std::cout << generateCenteredText("Welcome to the Chat Room Creation Scene!");
	drawLine(1);
	std::cout << std::endl;
	std::cout << generateCenteredText("What username would you like to use?") << std::endl;
	std::cout << generateCenteredText("(Maximum of " + std::to_string(sMAX_USERNAME_LENGTH) + " characters)") << std::endl << std::endl;
	std::cout << generateCenteredText("Or press ESC to go back to the Main Menu") << std::endl << std::endl;
}

void CreateChatRoomScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
		SceneManager::switchScene(SceneId::MAIN_MENU);
		return;
	}
	else if (input == ENTER_KEY)
	{
		if (mCurrentInput.length() <= sMAX_USERNAME_LENGTH && mCurrentInput.length() > 0)
		{
			SceneManager::switchScene(SceneId::CHATROOM);
			return;
		}
		else
		{
			clearInput();
			clearScreenPortion(0, getConsoleCursorY(), getConsoleWidth(), 1);
			clearScreenPortion(0, getConsoleCursorY() + 1, getConsoleWidth(), 1);
			setCursorPosition(0, getConsoleCursorY() + 1);
			std::cerr << "Please enter a username shorter than: " + std::to_string(sMAX_USERNAME_LENGTH) + "!" << std::endl;
			setCursorPosition(0, getConsoleCursorY() + 1);
		}
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