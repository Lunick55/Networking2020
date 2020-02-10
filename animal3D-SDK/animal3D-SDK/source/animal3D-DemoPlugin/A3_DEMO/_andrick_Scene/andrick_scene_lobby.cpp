#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_lobby.h"
#include "../_andrick_Network/andrick_host.h"

LobbyScene::LobbyScene() :
	Scene(SceneId::Lobby),
	mCurrentStep(LobbyStep::CHATROOM),
	mSelectedGame(GameType::NONE)
{

}

void LobbyScene::enteringScene(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		Host::spInstance->startChatRoom(demoState);
	}
	else if (Client::isInitialized())
	{
		if (Client::spInstance->connectToServer())
		{
			addToChatList(MessageType::EITHER, "Trying to connect...");
		}
		else
		{
			addToChatList(MessageType::EITHER, "Failed to connect to server.");
		}
	}
	else
	{
		addToChatList(MessageType::EITHER, "Uh oh, something went wrong!");
	}
}

void LobbyScene::input(a3_DemoState* demoState)
{
	if (mCurrentStep == LobbyStep::CHATROOM)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = LobbyStep::LEAVE_SERVER_ARE_YOU_SURE;
			return;
		}
		else if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
		{
			if (demoState->currentKey == a3key_enter)
			{
				if (!mCurrentInput.empty())
				{
					//commands
					if (mCurrentInput[0] == '/')
					{
						std::size_t spaceIndex = mCurrentInput.find_first_of(' ');
						std::string currCommand = mCurrentInput.substr(1, spaceIndex - 1);

						if (currCommand == WHISPER_COMMAND)
						{
							std::string toUser;

							for (std::size_t i = spaceIndex + 1; i < mCurrentInput.length() - 1; ++i)
							{
								if (mCurrentInput[i] != ',')
								{
									toUser += mCurrentInput[i];
								}
								else
								{
									std::size_t startIndex = (i + 2);
									if (startIndex < mCurrentInput.length())
									{
										std::string secret = mCurrentInput.substr(startIndex);

										//send a private message
										if (Client::isHost())
										{
											Host::spInstance->deliverPersonalMessage(demoState, toUser, secret);
											mCurrentInput.clear();
										}
										else
										{
											Client::spInstance->sendPrivateMessageRequest(secret, toUser);
											mCurrentInput.clear();
										}

										return;
									}
								}
							}
						}
						else if (currCommand == START_GAME_COMMAND)
						{
							std::string input;
							for (std::string::size_type i = 0; i < mCurrentInput.length(); ++i)
								input += std::tolower(mCurrentInput[i]);

							if (input.find("tic") != std::string::npos)
							{
								//Go to tictactoe scene
								mSelectedGame = GameType::TICTAC;
								return;
							}
							else if (input.find("battle") != std::string::npos)
							{
								//Go to battleship scene
								mSelectedGame = GameType::BATTLESHIP;
								return;
							}
						}
						else if (currCommand == LIST_USERS)
						{
							mCurrentInput.clear();

							//List users
							if (Client::isHost())
							{
								Host::listUserInfoRequest(demoState);
								return;
							}
							else
							{
								addToChatList(MessageType::EITHER, "You don't have permission for this command!");
								return;
							}
						}
					}
					else
					{
						//Send chat message
						if (Client::isHost())
						{
							Host::spInstance->deliverPublicMessage(demoState, Host::spInstance->mpHost, mCurrentInput);
							mCurrentInput.clear();
						}
						else
						{
							Client::spInstance->sendPublicMessage(mCurrentInput);
							mCurrentInput.clear();
						}
					}

					mCurrentInput.clear();
				}
			}
			else if (demoState->currentKey == a3key_backspace && mCurrentInput.size() > 0)
			{
				mCurrentInput = mCurrentInput.substr(0, mCurrentInput.size() - 1);
			}
			else if (demoState->currentKey == a3key_period)
			{
				mCurrentInput += ".";
			}
			else if (demoState->currentKey == a3key_slash)
			{
				mCurrentInput += "/";
			}
			else
			{
				//This doesn't work for all keys since they're not completely mapped to ascii.
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	else if (mCurrentStep == LobbyStep::LEAVE_SERVER_ARE_YOU_SURE)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = LobbyStep::LEAVE_SERVER;
			return;
		}
		else
		{
			mCurrentStep = LobbyStep::CHATROOM;
			return;
		}
	}
}

void LobbyScene::networkReceive(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		Host::spInstance->update(demoState);
	}

	Client::spInstance->update(demoState);
}

void LobbyScene::update(const a3_DemoState* demoState)
{
	if (mSelectedGame == GameType::TICTAC)
	{
		//Go to tictactoe scene
		demoState->mpSceneManager->switchToScene(demoState, SceneId::Tictactoe);
		return;
	}
	else if (mSelectedGame == GameType::BATTLESHIP)
	{
		//Go to battleship scene
		demoState->mpSceneManager->switchToScene(demoState, SceneId::Battleship);
		return;
	}
}

void LobbyScene::render(const a3_DemoState* demoState)
{
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);

	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText(demoState, "Lobby Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	formatter.offsetLine(2);

	//There is no differentiation between players and spectators in the lobby scene.
	std::vector<LogInfo>::iterator iter = mChatLog.begin();
	for (; iter != mChatLog.end(); ++iter)
	{
		formatter.drawText(demoState, iter->text);
	}

	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput);
}


void LobbyScene::networkSend(const a3_DemoState* demoState)
{
	
}