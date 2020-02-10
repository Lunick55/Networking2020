#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_tictactoe.h"
#include "../_andrick_Network/andrick_host.h"

TictactoeScene::TictactoeScene() :
	Scene(SceneId::Tictactoe),
	mCurrentStep(TicTacStep::SELECT_PLAYERS),
	mPlayer(PlayerType::SPECTATOR),
	mPlayerSignature(gs_tictactoe_space_invalid),
	mBoardPosition(TextFormatter::createVec3(-0.8f, 0.5f + TextFormatter::LINE_HEIGHT * 2.0f, -1.0f)),
	mBoardStartY(0.5f + TextFormatter::LINE_HEIGHT * 2.0f),
	mPlayer1Id(INVALID_USER_ID),
	mPlayer2Id(INVALID_USER_ID),
	mPlayer1Username(),
	mPlayer2Username(),
	mPlayer1Position(TextFormatter::createVec3(-0.8f, 0.5f + TextFormatter::LINE_HEIGHT * 4.0f, -1.0f)),
	mPlayer2Position(TextFormatter::createVec3(-0.8f, 0.5f - TextFormatter::LINE_HEIGHT * 4.0f, -1.0f))
{
	gs_tictactoe_reset(mGame);
}

void TictactoeScene::enteringScene(const a3_DemoState* demoState)
{
	mCurrentStep = TicTacStep::SELECT_PLAYERS;
	mPlayer = PlayerType::SPECTATOR; //for now
	mPlayer1Id = INVALID_USER_ID;
	mPlayer2Id = INVALID_USER_ID;
	mPlayer1Username = "";
	mPlayer2Username = "";
}

void TictactoeScene::input(a3_DemoState* demoState)
{
	if(mCurrentStep == TicTacStep::SELECT_PLAYERS)
	{
		//pick player names - /players, Andy608, Lunick55
		if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
		{
			if (demoState->currentKey == a3key_enter)
			{
				if (!mCurrentInput.empty())
				{
					//commands
					if (mCurrentInput[0] == '/')
					{
						std::size_t delimiterIndex = mCurrentInput.find_first_of(' ');
						std::string currCommand = mCurrentInput.substr(1, delimiterIndex - 1);

						//	/players Andy608, Lunick55

						if (currCommand == SELECT_PLAYERS_COMMAND && Client::isHost())
						{
							std::string player1, player2;

							for (std::size_t i = delimiterIndex + 1; i < mCurrentInput.length(); ++i)
							{
								if (mCurrentInput[i] != ',')
								{
									player1 += mCurrentInput[i];
								}
								else
								{
									delimiterIndex = i;
									break;
								}
							}

							if (delimiterIndex + 1 < mCurrentInput.length())
							{
								player2 = mCurrentInput.substr(delimiterIndex + 1);

								//Start game
								setupPlayers(player1, player2);
							}
							else
							{
								//invalid player 2
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
			else if (demoState->currentKey == a3key_comma)
			{
				mCurrentInput += ",";
			}
			else
			{
				//This doesn't work for all keys since they're not completely mapped to ascii.
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	else if (mCurrentStep == TicTacStep::YOUR_TURN || mCurrentStep == TicTacStep::NOT_YOUR_TURN || mCurrentStep == TicTacStep::SPECTATING)
	{
		/*if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = TictactoeScene::LEAVE_SERVER_ARE_YOU_SURE;
			return;
		}*/
		if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
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
						else if (currCommand == PLAY_TURN_COMMAND && mCurrentStep == TicTacStep::YOUR_TURN)
						{
							//    /PLAY 8 - extracting the 8 from the command
							std::string boardSpace = mCurrentInput.substr(spaceIndex + 1, 1);

							try
							{
								int boardIndex = std::stoi(boardSpace);

								if (boardIndex > 0 && boardIndex <= 9)
								{
									gs_tictactoe_space_state result = gs_tictactoe_space_invalid;

									//We have the space.
									if (isKeyPressed(demoState, a3key_numpad1))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 0, 0);
									else if (isKeyPressed(demoState, a3key_numpad2))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 1, 0);
									else if (isKeyPressed(demoState, a3key_numpad3))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 2, 0);
									else if (isKeyPressed(demoState, a3key_numpad4))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 0, 1);
									else if (isKeyPressed(demoState, a3key_numpad5))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 1, 1);
									else if (isKeyPressed(demoState, a3key_numpad6))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 2, 1);
									else if (isKeyPressed(demoState, a3key_numpad7))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 0, 2);
									else if (isKeyPressed(demoState, a3key_numpad8))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 1, 2);
									else if (isKeyPressed(demoState, a3key_numpad8))
										result = gs_tictactoe_setSpaceState(mGame, mPlayerSignature, 2, 2);

									if (result == gs_tictactoe_space_invalid)
									{
										//Invalid move! Go again.
										throw std::invalid_argument("That space is already taken! Go again.");
									}
									else
									{
										mCurrentStep = TicTacStep::NOT_YOUR_TURN;
									}
								}
								else
								{
									throw std::invalid_argument("Invalid move! Go again.");
								}
							}
							catch (const std::invalid_argument& e)
							{
								//The user did not type in a valid number.
								addToChatList(MessageType::PLAYER, e.what(), 1, TextFormatter::RED);
								return;
							}

							//TODO: send the packet to everyone containing gameState, and whoseTurn or whatever
							char game[3][3];
							memcpy(game, mGame, sizeof(char) * 9);
							UpdateTicTacState updatePacket = UpdateTicTacState(Client::spInstance->mpClient->getUserId(), game);

							if (Client::isHost())
							{
								//broadcast
								Host::spInstance->broadcastPacket((const char*)(&updatePacket), sizeof(UpdateTicTacState));
							}
							else
							{
								Client::spInstance->mpPeer->Send((const char*)(&updatePacket), sizeof(UpdateTicTacState),
									PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
									0, Client::spInstance->mHostAddress, false);
							}

							return;
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
						//broadcastMessage(demoState, mCurrentInput);
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
			else if (demoState->currentKey == a3key_comma)
			{
				mCurrentInput += ",";
			}
			else
			{
				//This doesn't work for all keys since they're not completely mapped to ascii.
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	//else if (mCurrentStep == TicTacStep::LEAVE_SERVER_ARE_YOU_SURE)
	//{
	//	if (isKeyPressed(demoState, a3key_Y))
	//	{
	//		mCurrentStep = TicTacStep::LEAVE_SERVER;
	//		return;
	//	}
	//	else if (isKeyPressed(demoState, a3key_N))
	//	{
	//		mCurrentStep = TicTacStep::CHATROOM;
	//		return;
	//	}
	//}
	//else if (mCurrentStep == TicTacStep::RESET)
	//{
	//
	//}
	//else if (mCurrentStep == TicTacStep::EXIT)
	//{
	//
	//}
}

void TictactoeScene::networkReceive(const a3_DemoState* demoState)
{
	//Scene::networkReceive(demoState);

	//Incoming packets to server from client.
	for (Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive(); Client::spInstance->mpPacket; Client::spInstance->mpPeer->DeallocatePacket(Client::spInstance->mpPacket), Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive())
	{
		switch (Client::spInstance->mpPacket->data[0])
		{
			case PacketEventId::DELIVER_PUBLIC_MESSAGE:
			{
				DeliverPublicMessagePacket* data = (DeliverPublicMessagePacket*)(Client::spInstance->mpPacket->data);

				//TODO: Call message to print to console.
				//std::cout << data->message << std::endl;
				addToChatList((MessageType)data->msgType, std::string(data->message));
				break;
			}
			case PacketEventId::SEND_PUBLIC_MESSAGE_REQUEST:
			{
				SendPublicMessageRequestPacket* data = (SendPublicMessageRequestPacket*)(Client::spInstance->mpPacket->data);

				SendPublicMessageRequestPacket messagePacket = SendPublicMessageRequestPacket(data->userId, data->message);

				std::shared_ptr<User> user = Host::spInstance->getUserFromId(data->userId);
				if (user != nullptr)
				{
					Host::spInstance->deliverPublicMessage(demoState, user, data->message);
				}
				else
				{
					//Couldn't find user!!!
				}

				break;
			}
			case PacketEventId::UPDATE_TICTAC_STATE:
			{
				UpdateTicTacState* updatedPacket = (UpdateTicTacState*)(Client::spInstance->mpPacket->data);

				for (int i = 0; i < GS_TICTACTOE_BOARD_HEIGHT; i++)
				{
					for (int j = 0; i < GS_TICTACTOE_BOARD_WIDTH; i++)
					{
						gs_tictactoe_setSpaceState(mGame, (gs_tictactoe_space_state)updatedPacket->tictactoeboard[i][j], i, j);
					}
				}

				if (Client::isHost())
				{
					Host::spInstance->broadcastPacket((const char*)(&Client::spInstance->mpPacket), sizeof(UpdateTicTacState));
				}

				if (updatedPacket->fromUserId == mPlayer1Id && mPlayer == PlayerType::PLAYER2)
				{
					addToChatList(MessageType::PLAYER, "It's your turn!", 1, TextFormatter::RED);
					mCurrentStep = TicTacStep::YOUR_TURN;
				}
				else if (updatedPacket->fromUserId == mPlayer2Id && mPlayer == PlayerType::PLAYER1)
				{
					addToChatList(MessageType::PLAYER, "Your turn has ended.", 1, TextFormatter::RED);
					mCurrentStep = TicTacStep::NOT_YOUR_TURN;
				}
			}
			case PacketEventId::SETUP_TICTAC_GAME:
			{
				SetupTictacGame* setupTictacPacket = (SetupTictacGame*)(Client::spInstance->mpPacket->data);

				mPlayer1Id = setupTictacPacket->player1Id;
				mPlayer2Id = setupTictacPacket->player2Id;
				mPlayer1Username = setupTictacPacket->player1Username;
				mPlayer2Username = setupTictacPacket->player2Username;

				if (Client::spInstance->mpClient->getUserId() == mPlayer1Id)
				{
					mPlayer = PlayerType::PLAYER1;
					//You are player 1!
					addToChatList(MessageType::PLAYER, "You are player 1! Congrats! - X", 2, TextFormatter::RED);
					addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::RED);
					mCurrentStep = TicTacStep::YOUR_TURN;
				}
				else if (Client::spInstance->mpClient->getUserId() == mPlayer2Id)
				{
					mPlayer = PlayerType::PLAYER2;
					addToChatList(MessageType::PLAYER, "You are player 2! Congrats! - O", 2, TextFormatter::RED);
					addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::RED);
					mCurrentStep = TicTacStep::NOT_YOUR_TURN;
				}
				else
				{
					mPlayer = PlayerType::SPECTATOR;
					addToChatList(MessageType::SPECTOR, "You are a spectator :)", 2, TextFormatter::RED);
					mCurrentStep = TicTacStep::SPECTATING;
				}
			}
			default:
				break;
		}
	}
}

void TictactoeScene::update(const a3_DemoState* demoState)
{

}

void TictactoeScene::render(const a3_DemoState* demoState)
{
	glClearColor(1.0f, 0.5f, 0.2f, 1.0f);

	TextFormatter& formatter = TextFormatter::get();
	formatter.setAlignment(TextFormatter::TextAlign::LEFT);

	formatter.setLine(0);
	formatter.drawText(demoState, "Tictactoe Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);

	formatter.drawText(demoState, "Player1: " + mPlayer1Username, TextFormatter::WHITE, TextFormatter::TextAlign::UNALIGN, mPlayer1Position);
	formatter.drawText(demoState, "Player2: " + mPlayer2Username, TextFormatter::WHITE, TextFormatter::TextAlign::UNALIGN, mPlayer2Position);

	//Draw game board
	std::string row;
	for (int i = 0; i < 2; ++i)
	{
		// X | X | X
		//---|---|---
		// X | X | X
		//---|---|---

		row = std::string(std::string(" ") + getXOSpace(i, 0) + std::string(" | ") + getXOSpace(i, 1) + std::string(" | ") + getXOSpace(i, 2));
		formatter.drawText(demoState, row, TextFormatter::WHITE, TextFormatter::TextAlign::UNALIGN, mBoardPosition);
		mBoardPosition.y -= TextFormatter::LINE_HEIGHT;
		formatter.drawText(demoState, "---|---|---", TextFormatter::WHITE, TextFormatter::TextAlign::UNALIGN, mBoardPosition);
		mBoardPosition.y -= TextFormatter::LINE_HEIGHT;
	}

	row = std::string(std::string(" ") + getXOSpace(0, 0) + std::string(" | ") + getXOSpace(1, 1) + std::string(" | ") + getXOSpace(2, 2));
	formatter.drawText(demoState, row, TextFormatter::WHITE, TextFormatter::TextAlign::UNALIGN, mBoardPosition);

	mBoardPosition.y = mBoardStartY;

	//Messages
	formatter.setLine(4);
	formatter.setAlignment(TextFormatter::TextAlign::RIGHT);

	std::vector<LogInfo>::iterator iter = mChatLog.begin();
	for (; iter != mChatLog.end(); ++iter)
	{
		//Message for everyone
		if (iter->type == MessageType::EITHER)
		{
			formatter.drawText(demoState, iter->text, iter->color);
		}
		else if (iter->type == MessageType::PLAYER && mPlayer == PlayerType::PLAYER1 || mPlayer == PlayerType::PLAYER2)
		{
			formatter.drawText(demoState, iter->text, iter->color);
		}
		else if (iter->type == MessageType::SPECTOR && mPlayer == PlayerType::SPECTATOR)
		{
			formatter.drawText(demoState, iter->text, iter->color);
		}
	}

	//if (mCurrentStep == LobbyStep::LEAVE_SERVER_ARE_YOU_SURE)
	//{
	//	formatter.drawText(demoState, "Are you sure you want to leave the server? (Y/N)");
	//}

	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE, TextFormatter::TextAlign::RIGHT);
}

char TictactoeScene::getXOSpace(int i, int j)
{
	if (mGame[i][j] == gs_tictactoe_space_o)
		return 'O';
	else if (mGame[i][j] == gs_tictactoe_space_x)
		return 'X';
	else if (mGame[i][j] == gs_tictactoe_space_open)
		return ' ';

	return ' ';
}


void TictactoeScene::networkSend(const a3_DemoState* demoState)
{

}

//Can only ever be called from the host
bool TictactoeScene::setupPlayers(std::string player1, std::string player2)
{
	if (player1.compare(player2) == 0)
	{
		//Players cant be the same;
		return false;
	}
	else
	{
		std::map<UserId, std::shared_ptr<User>>::iterator iter = Host::spInstance->mpConnectedUsers.begin();
		UserId player1Id = -1;
		UserId player2Id = -1;

		for (; iter != Host::spInstance->mpConnectedUsers.end(); ++iter)
		{
			if (iter->second->getUsername().compare(player1) == 0)
			{
				player1Id = iter->first;
				break;
			}
		}

		iter = Host::spInstance->mpConnectedUsers.begin();
		for (; iter != Host::spInstance->mpConnectedUsers.end(); ++iter)
		{
			if (iter->second->getUsername().compare(player2) == 0)
			{
				player2Id = iter->first;
				break;
			}
		}

		//Tell everyone who the players are and start the game
		if (player1Id != INVALID_USER_ID && player2Id != INVALID_USER_ID)
		{
			if (player1Id == Host::spInstance->mpHost->getUserId())
			{
				mCurrentStep = TicTacStep::YOUR_TURN;
			}
			else if (player2Id == Host::spInstance->mpHost->getUserId())
			{
				mCurrentStep = TicTacStep::NOT_YOUR_TURN;
			}

			SetupTictacGame setupTictacPacket = SetupTictacGame(
				player1Id, Host::spInstance->mpConnectedUsers.at(player1Id)->getUsername(),
				player2Id, Host::spInstance->mpConnectedUsers.at(player2Id)->getUsername());

			Host::spInstance->broadcastPacket((const char*)(&setupTictacPacket), sizeof(SetupTictacGame));
		}

		//Setup game for host
		mPlayer1Id = player1Id;
		mPlayer2Id = player2Id;
	}
	return false;
}

void TictactoeScene::broadcastMessage(const a3_DemoState* demoState, std::string message)
{
	MessageType type = MessageType::EITHER;

	switch (mPlayer)
	{
	case PlayerType::PLAYER1:
	case PlayerType::PLAYER2:
		type = MessageType::PLAYER;
		break;
	case PlayerType::SPECTATOR:
		type = MessageType::SPECTOR;
		break;
	}

	if (Client::isHost())
	{
		DeliverPublicMessagePacket deliverPacket = DeliverPublicMessagePacket(Host::spInstance->mpHost->getUserId(), message, type);
		Host::spInstance->broadcastPacket((const char*)(&deliverPacket), sizeof(DeliverPublicMessagePacket));
	}
	else
	{
		Client::spInstance->sendPublicMessage(message, type);
	}
}