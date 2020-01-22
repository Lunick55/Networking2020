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
	std::cout << generateCenteredText("How many clients do you want to allow on your server?") << std::endl << std::endl;
	std::cout << generateCenteredText("Or press ESC to go back to the Main Menu") << std::endl << std::endl;
}

void CreateChatRoomScene::handleInput(const char& input)
{
	if (input == ESC_KEY)
	{
		if (mStepCount <= ENTER_MAX_USERS)
		{
			mStepCount = ENTER_MAX_USERS;
			SceneManager::switchScene(SceneId::MAIN_MENU);
			return;
		}
		else if (mStepCount == ENTER_USERNAME)
		{
			mStepCount = ENTER_MAX_USERS;
			mUsername = "";
			clearScreenPortion(0, mSTEP1_CLEAR_START_HEIGHT, getConsoleWidth(), mSTEP1_CLEAR_HEIGHT);
			setCursorPosition(0, getConsoleCursorY() - mSTEP1_CLEAR_HEIGHT + 1);
		}
	}
	else if (input == ENTER_KEY)
	{
		if (mStepCount == ENTER_MAX_USERS)
		{
			try
			{
				mMaxUsers = std::stoi(mCurrentInput);
				clearInput();
			}
			catch (...)
			{
				clearInput();
				clearScreenPortion(0, getConsoleCursorY(), getConsoleWidth(), 1);
				setCursorPosition(0, getConsoleCursorY());
				std::cout << "Invalid number. Using default maximum of: " << std::to_string(sDEFAULT_MAX_USERS) << std::endl;
				mMaxUsers = sDEFAULT_MAX_USERS;
			}

			++mStepCount;
			std::cout << std::endl;

			std::cout << generateCenteredText("What username would you like to use?") << std::endl;
			std::cout << generateCenteredText("(Maximum of " + std::to_string(sMAX_USERNAME_LENGTH) + " characters)") << std::endl << std::endl;
		}
		else if (mStepCount == ENTER_USERNAME)
		{
			if (mCurrentInput.length() <= sMAX_USERNAME_LENGTH && mCurrentInput.length() > 0)
			{
				mUsername = mCurrentInput;
				clearInput();
				initChatRoom();
				mStepCount = ENTER_MAX_USERS;
				SceneManager::switchScene(SceneId::CHATROOM);
				return;
			}
			else
			{
				clearInput();
				clearScreenPortion(0, mSTEP2_CLEAR_START_HEIGHT, getConsoleWidth(), mSTEP2_CLEAR_HEIGHT);
				setCursorPosition(0, getConsoleCursorY() - mSTEP2_CLEAR_HEIGHT + 1);
				std::cerr << "Please enter a username shorter than: " + std::to_string(sMAX_USERNAME_LENGTH) + "!" << std::endl;
				setCursorPosition(0, getConsoleCursorY() + 1);
			}
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

void CreateChatRoomScene::initChatRoom()
{

}