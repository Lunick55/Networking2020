#include "Scene.h"
#include "SceneManager.h"

void MenuScene::update()
{
}

void MenuScene::render()
{
}

void MenuScene::drawInitialScene()
{
	std::cout << generateCenteredText("Welcome to the Main Menu!");
	drawLine(1);
	std::cout << std::endl;
	std::cout << generateCenteredText("Press (1) to host a server.") << std::endl;
	std::cout << generateCenteredText("Press (2) to join a server.") << std::endl;
	std::cout << generateCenteredText("Press ESC to close the application.") << std::endl;
}

void MenuScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
		requestQuit();
		return;
	}
	else if (input == ONE)
	{
		SceneManager::switchScene(SceneId::CREATE_CHATROOM);
	}
	else if (input == TWO)
	{
		SceneManager::switchScene(SceneId::JOIN_CHATROOM);
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
