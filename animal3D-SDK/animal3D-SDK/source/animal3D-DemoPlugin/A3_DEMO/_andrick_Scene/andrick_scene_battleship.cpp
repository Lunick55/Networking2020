#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_battleship.h"

BattleShipScene::BattleShipScene() :
	Scene(SceneId::Battleship)
{

}

void BattleShipScene::enteringScene(const a3_DemoState* demoState)
{
	mCurrentStep = BattleStep::SELECT_PLAYERS;
	mPlayer = PlayerType::SPECTATOR; //for now
}


void BattleShipScene::input(a3_DemoState* demoState)
{
	//if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
	//{
	//	demoState->mpSceneManager->switchToScene(demoState, SceneId::SelectRole);
	//}
	if (mCurrentStep == BattleStep::SELECT_PLAYERS)
	{
		//pick player names - /players, Andy608, Lunick55
		if (Client::isHost())
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

						if (currCommand == SELECT_PLAYERS_COMMAND)
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
								mCurrentStep = BattleStep::START_GAME;
								setupPlayers(player1, player2);
							}
							else
							{
								//invalid player 2
							}
						}
					}
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (mCurrentStep == BattleStep::YOUR_TURN || mCurrentStep == BattleStep::NOT_YOUR_TURN)
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
						else if (currCommand == PLAY_TURN_COMMAND && mCurrentStep == BattleStep::YOUR_TURN)
						{
							//		/PLAY B8 - extracting the B & 8 from the command
							std::string boardSpace = mCurrentInput.substr(spaceIndex + 1, 2);
							// A-J = 65-74	 |	 1-10
							try
							{
								unsigned char boardLetter = boardSpace[0];
								unsigned char boardNum = boardSpace[1];

								if (gs_checkers_getSpaceState(mGame, 1, boardLetter, boardNum) == gs_battleship_space_state::gs_battleship_space_open)
								{
									//Asks opponent if we hit them or not
									char game[2] = {(char)boardLetter, (char)boardNum};
									AskIfBattleHit askPacket = AskIfBattleHit(Client::spInstance->mpClient->getUserId(), game);
									
									if (Client::isHost())
									{
										//broadcast. PLAYERS ARE ONLY ONES WHO CARE
										Host::spInstance->broadcastPacket((const char*)(&askPacket), sizeof(UpdateBattleState));
									}
									else
									{
										//broadcast. PLAYERS ARE ONLY ONES WHO CARE
										Client::spInstance->mpPeer->Send((const char*)(&askPacket), sizeof(UpdateBattleState),
											PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
											0, Client::spInstance->mHostAddress, false);
									}

									mCurrentStep = BattleStep::NOT_YOUR_TURN;
								}
								else
								{
									throw std::invalid_argument("Invalid space!");
								}
							}
							catch (...)
							{
								//The user did not type in a valid number.
							}

							//send the packet to everyone containing gameState, and whoseTurn or whatever. PLAYERS SHOULD NOT RECIEVE
							char game[GS_BATTLESHIP_PLAYERS][GS_BATTLESHIP_BOARD_WIDTH][GS_BATTLESHIP_BOARD_HEIGHT];
							memcpy(game, mGame, sizeof(char) * (GS_BATTLESHIP_PLAYERS * GS_BATTLESHIP_BOARD_WIDTH * GS_BATTLESHIP_BOARD_HEIGHT));
							UpdateBattleState updatePacket = UpdateBattleState(Client::spInstance->mpClient->getUserId(), game);

							if (Client::isHost())
							{
								//broadcast
								Host::spInstance->broadcastPacket((const char*)(&updatePacket), sizeof(UpdateBattleState));
							}
							else
							{
								Client::spInstance->mpPeer->Send((const char*)(&updatePacket), sizeof(UpdateBattleState),
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
}

void BattleShipScene::networkReceive(const a3_DemoState* demoState)
{
	//Scene::networkReceive(demoState);
	//Incoming packets to server from client.
	for (Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive(); Client::spInstance->mpPacket; Client::spInstance->mpPeer->DeallocatePacket(Client::spInstance->mpPacket), Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive())
	{
		switch (Client::spInstance->mpPacket->data[0])
		{
		case PacketEventId::UPDATE_BATTLE_STATE:
		{
			if (Client::isHost())
			{
				Host::spInstance->broadcastPacket((const char*)(&Client::spInstance->mpPacket), sizeof(UpdateBattleState));
			}

			if (mPlayer != PlayerType::SPECTATOR)
			{
				//Not for player eyes
				return;
			}

			UpdateBattleState* updatedPacket = (UpdateBattleState*)(Client::spInstance->mpPacket->data);

			for (int i = 0; i < GS_BATTLESHIP_BOARD_HEIGHT; i++)
			{
				for (int j = 0; i < GS_BATTLESHIP_BOARD_WIDTH; i++)
				{
					//if player 1
					gs_checkers_setSpaceState(mGame, (gs_battleship_space_state)0, (gs_battleship_space_state)updatedPacket->battleBoard[0][i][j], i, j);

					//if player 2
					gs_checkers_setSpaceState(mGame, (gs_battleship_space_state)1, (gs_battleship_space_state)updatedPacket->battleBoard[1][i][j], i, j);
				}
			}
		}
		case PacketEventId::SETUP_BATTLE_GAME:
		{

		}
		case PacketEventId::ASK_IF_BATTLE_HIT:
		{
			if (Client::isHost())
			{
				Host::spInstance->broadcastPacket((const char*)(&Client::spInstance->mpPacket), sizeof(AskIfBattleHit));
			}

			if (mPlayer == PlayerType::SPECTATOR)
			{
				//Not for Spector eyes
				return;
			}


		}
		case PacketEventId::REPLY_IF_BATTLE_HIT:
		{
			if (Client::isHost())
			{
				Host::spInstance->broadcastPacket((const char*)(&Client::spInstance->mpPacket), sizeof(ReplyIfBattleHit));
			}

			if (mPlayer == PlayerType::SPECTATOR)
			{
				//Not for Spector eyes
				return;
			}

		}
		default:
			break;
		}
	}
}

void BattleShipScene::update(const a3_DemoState* demoState)
{

}

void BattleShipScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.4f, 1.0f, 0.2f, 1.0f);
	
	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText(demoState, "Battleship Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	formatter.offsetLine(2);
}


void BattleShipScene::networkSend(const a3_DemoState* demoState)
{

}

bool BattleShipScene::setupPlayers(std::string player1, std::string player2)
{
	//TODO: populate

	return false;
}