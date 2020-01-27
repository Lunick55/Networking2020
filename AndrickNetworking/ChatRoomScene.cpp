#include "ChatRoomScene.h"
#include "SceneManager.h"
#include "ChatRoomServer.h"
#include "ChatRoomClient.h"
#include "User.h"

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
			ChatRoomScene::printMessageToChatRoom("Trying to connect...");
		}
		else
		{
			ChatRoomScene::printMessageToChatRoom("Failed to connect to server.");
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

		///   /whisper Suck me fuck me, I love you
		if (mCurrentInput[0] == '/')
		{
			std::size_t spaceIndex = mCurrentInput.find_first_of(' ');
			std::string currCommand = mCurrentInput.substr(1, spaceIndex - 1);

			if(currCommand == WHISPER_COMMAND)
			{
				std::string toUser;

				for (int i = spaceIndex + 1; i < mCurrentInput.length(); ++i)
				{
					if (mCurrentInput[i] != ',')
					{
						toUser += mCurrentInput[i];
					}
					else
					{
						//send a private message
						if (ChatRoomClient::isHost())
						{
							//ChatRoomServer::spInstance->deliverPrivateMessage(ChatRoomServer::spInstance->mpHost->getUserId(), mCurrentInput);
						}
						else
						{
							ChatRoomClient::spInstance->sendPrivateMessageRequest(mCurrentInput, toUser);
						}

						return;
					}
				}
			}
		}

		if (ChatRoomClient::isHost())
		{
			ChatRoomServer::spInstance->deliverPublicMessage(ChatRoomServer::spInstance->mpHost, mCurrentInput);
		}
		else
		{
			ChatRoomClient::spInstance->sendPublicMessage(mCurrentInput);
		}
		clearInput();
		//setCursorPosition(0, getConsoleCursorY());
	}
	else if (input == BACKSPACE_KEY)
	{
		if (!mCurrentInput.empty())
		{
			mCurrentInput.pop_back();
			setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
			std::cout << " ";
			setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
		}
	}
	else
	{
		setCursorPosition(mCurrentInput.length(), getConsoleCursorY());
		mCurrentInput += input;
		std::cout << input;
	}
}

void ChatRoomScene::printMessageToChatRoom(const std::string& message)
{
	SceneManager instance = SceneManager::instance();
	std::shared_ptr<ChatRoomScene> chatInstance = instance.mpChatRoomScene;

	clearScreenPortion(0, getConsoleCursorY(), getConsoleWidth(), 1);
	setCursorPosition(0, getConsoleCursorY());

	std::cout << message << std::endl;
	std::cout << chatInstance->mCurrentInput;
}