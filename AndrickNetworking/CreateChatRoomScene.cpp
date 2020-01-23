#include "Scene.h"
#include "SceneManager.h"
#include "ChatRoomServer.h"
#include <exception>

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
	std::cout << generateCenteredText("How many clients do you want to allow on your server? (Max of 64)") << std::endl << std::endl;
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

				if (mMaxUsers > sMAX_USERS)
				{
					clearScreenPortion(0, mSTEP1_CLEAR_START_HEIGHT, getConsoleWidth(), mSTEP1_CLEAR_HEIGHT);
					setCursorPosition(0, getConsoleCursorY() - mSTEP1_CLEAR_HEIGHT + 1);
					std::cerr << "Please enter a number less than " + std::to_string(sMAX_USERS) + "!" << std::endl;
					setCursorPosition(0, getConsoleCursorY() + 1);
				}
				else
				{
					++mStepCount;
					std::cout << std::endl;
					std::cout << generateCenteredText("What username would you like to use?") << std::endl;
					std::cout << generateCenteredText("(Maximum of " + std::to_string(sMAX_USERNAME_LENGTH) + " characters)") << std::endl << std::endl;
				}
			}
			catch (...)
			{
				clearInput();
				clearScreenPortion(0, mSTEP1_CLEAR_START_HEIGHT, getConsoleWidth(), mSTEP1_CLEAR_HEIGHT);
				setCursorPosition(0, getConsoleCursorY() - mSTEP1_CLEAR_HEIGHT + 1);
				std::cout << "Invalid number. Please enter another number." << std::endl;
				mMaxUsers = sDEFAULT_MAX_USERS;
			}
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
	ChatRoomServer::initChatRoom(sPORT, mMaxUsers, mUsername);
}