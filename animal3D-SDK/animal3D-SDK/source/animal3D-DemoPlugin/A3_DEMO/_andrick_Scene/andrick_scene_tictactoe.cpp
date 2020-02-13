#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_tictactoe.h"
#include "../_andrick_Network/andrick_host.h"

TictactoeScene::TictactoeScene() :
	Scene(SceneId::Tictactoe),
	mCurrentStep(TicTacStep::SELECT_PLAYERS),
	mPlayerType(PlayerType::SPECTATOR),
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
	gs_tictactoe_reset(mTictacBoard);

	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad1, {2, 0} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad2, {2, 1} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad3, {2, 2} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad4, {1, 0} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad5, {1, 1} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad6, {1, 2} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad7, {0, 0} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad8, {0, 1} });
	mKeypadToBoardMap.insert({ a3_KeyboardKey::a3key_numpad9, {0, 2} });

	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_1, {2, 0} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_2, {2, 1} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_3, {2, 2} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_4, {1, 0} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_5, {1, 1} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_6, {1, 2} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_7, {0, 0} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_8, {0, 1} });
	mNumbersToBoardMap.insert({ a3_KeyboardKey::a3key_9, {0, 2} });
}

void TictactoeScene::enteringScene(const a3_DemoState* demoState)
{
	Scene::enteringScene(demoState);

	mCurrentStep = TicTacStep::SELECT_PLAYERS;
	mPlayerType = PlayerType::SPECTATOR;
	mPlayer1Id = INVALID_USER_ID;
	mPlayer2Id = INVALID_USER_ID;
	mPlayer1Username.clear();
	mPlayer2Username.clear();
}

void TictactoeScene::input(a3_DemoState* demoState)
{
	switch (mCurrentStep)
	{
	case TicTacStep::SPECTATOR_LEAVE_SERVER_CONFIRM:
		handleInputSpectatorLeaveServerConfirm(demoState);
		break;
	case TicTacStep::OPPONENT_LEAVE_SERVER_CONFIRM:
		handleInputOpponentLeaveServerConfirm(demoState);
		break;
	case TicTacStep::SELECT_PLAYERS:
		handleInputSelectPlayers(demoState);
		break;
	case TicTacStep::YOUR_TURN:
		handleInputYourTurn(demoState);
		break;
	case TicTacStep::OPPONENTS_TURN:
		handleInputOpponentsTurn(demoState);
		break;
	case TicTacStep::SPECTATOR:
		handleInputSpectator(demoState);
		break;
	default:
		break;
	}
}

const std::pair<int, int> TictactoeScene::getIndexOnBoard(a3_KeyboardKey key)
{
	auto iter = mKeypadToBoardMap.begin();
	for (auto&& iter : mKeypadToBoardMap)
	{
		if (iter.first == key)
		{
			return iter.second;
		}
	}

	iter = mNumbersToBoardMap.begin();
	for (auto&& iter : mNumbersToBoardMap)
	{
		if (iter.first == key)
		{
			return iter.second;
		}
	}

	return std::pair<int, int>(-1, -1);
}

TictactoeScene::PlayerType TictactoeScene::isWin()
{
	bool threeInARow;
	for (int i = 0; i < 3; ++i)
	{
		threeInARow = true;
		for (int j = 0; j < 3; ++j)
		{
			if (mTictacBoard[i][j] != gs_tictactoe_space_o)
			{
				threeInARow = false;
			}
		}

		if (threeInARow)
		{
			return TictactoeScene::PlayerType::PLAYER2;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		threeInARow = true;
		for (int j = 0; j < 3; ++j)
		{
			if (mTictacBoard[j][i] != gs_tictactoe_space_o)
			{
				threeInARow = false;
			}
		}

		if (threeInARow)
		{
			return TictactoeScene::PlayerType::PLAYER2;
		}
	}

	if (mTictacBoard[0][0] == gs_tictactoe_space_o
		&& mTictacBoard[1][1] == gs_tictactoe_space_o
		&& mTictacBoard[2][2] == gs_tictactoe_space_o
		||
		mTictacBoard[0][2] == gs_tictactoe_space_o
		&& mTictacBoard[1][1] == gs_tictactoe_space_o
		&& mTictacBoard[2][0] == gs_tictactoe_space_o)
	{
		threeInARow = true;
	}

	if (threeInARow)
	{
		return TictactoeScene::PlayerType::PLAYER2;
	}

	for (int i = 0; i < 3; ++i)
	{
		threeInARow = true;
		for (int j = 0; j < 3; ++j)
		{
			if (mTictacBoard[i][j] != gs_tictactoe_space_x)
			{
				threeInARow = false;
			}
		}

		if (threeInARow)
		{
			return TictactoeScene::PlayerType::PLAYER1;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		threeInARow = true;
		for (int j = 0; j < 3; ++j)
		{
			if (mTictacBoard[j][i] != gs_tictactoe_space_x)
			{
				threeInARow = false;
			}
		}

		if (threeInARow)
		{
			return TictactoeScene::PlayerType::PLAYER1;
		}
	}

	if (mTictacBoard[0][0] == gs_tictactoe_space_x
		&& mTictacBoard[1][1] == gs_tictactoe_space_x
		&& mTictacBoard[2][2] == gs_tictactoe_space_x
		||
		mTictacBoard[0][2] == gs_tictactoe_space_x
		&& mTictacBoard[1][1] == gs_tictactoe_space_x
		&& mTictacBoard[2][0] == gs_tictactoe_space_x)
	{
		threeInARow = true;
	}

	if (threeInARow)
	{
		return TictactoeScene::PlayerType::PLAYER1;
	}

	//No win yet
	return TictactoeScene::PlayerType::SPECTATOR;
}

void TictactoeScene::finishTurn(const a3_DemoState* demoState)
{
	if (Client::isHost())
	{
		UpdateTicTacState updatePacket = UpdateTicTacState(Host::spInstance->mpHost->getUserId(), mTictacBoard);
		Host::spInstance->broadcastPacket((const char*)(&updatePacket), sizeof(UpdateTicTacState));
	}
	else
	{
		UpdateTicTacState updatePacket = UpdateTicTacState(Client::spInstance->mpClient->getUserId(), mTictacBoard);
		Client::spInstance->mpPeer->Send((const char*)(&updatePacket), sizeof(UpdateTicTacState),
			PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
			0, Client::spInstance->mHostAddress, false);
	}
	if (isWin() == TictactoeScene::PlayerType::PLAYER2)
	{
		addToChatList(MessageType::PLAYER, mPlayer2Username + " won!", 1, TextFormatter::YELLOW);
		mCurrentStep = TicTacStep::SELECT_PLAYERS;
		gs_tictactoe_reset(mTictacBoard);
	}
	else if ((isWin() == TictactoeScene::PlayerType::PLAYER1))
	{
		addToChatList(MessageType::PLAYER, mPlayer1Username + " won!", 1, TextFormatter::YELLOW);
		mCurrentStep = TicTacStep::SELECT_PLAYERS;
		gs_tictactoe_reset(mTictacBoard);
	}
	else
	{
		mCurrentStep = TicTacStep::OPPONENTS_TURN;
	}
}

bool TictactoeScene::handleInputEscape(const a3_DemoState* demoState, TicTacStep targetStep)
{
	if (isKeyPressed(demoState, a3key_escape))
	{
		mCurrentStep = targetStep;
		return true;
	}

	return false;
}
void TictactoeScene::handleInputSpectatorLeaveServerConfirm(a3_DemoState* demoState)
{
	//if (isKeyPressed(demoState, a3key_Y))
	//{
	//	mCurrentStep = TicTacStep::LEAVE_SERVER;
	//}
	//else if (isKeyPressed(demoState, a3key_N))
	//{
	//	mCurrentStep = TicTacStep::SPECTATOR;
	//}
}
void TictactoeScene::handleInputOpponentLeaveServerConfirm(a3_DemoState* demoState)
{
	//if (isKeyPressed(demoState, a3key_Y))
	//{
	//	mCurrentStep = TicTacStep::LEAVE_SERVER;
	//}
	//else if (isKeyPressed(demoState, a3key_N))
	//{
	//	mCurrentStep = TicTacStep::;
	//}
}
void TictactoeScene::handleInputSelectPlayers(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, TicTacStep::SPECTATOR_LEAVE_SERVER_CONFIRM)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandResult;
			if (processCommand(demoState, mCurrentInput, command, commandResult))
			{
				if (command.compare(SELECT_PLAYERS_COMMAND) == 0)
				{
					std::string player1, player2;
					player1 = commandResult.substr(0, commandResult.find_first_of(','));
					player2 = commandResult.substr(commandResult.find_first_of(',') + 1);

					if (!setupPlayers(player1, player2))
					{
						addToChatList(MessageType::EITHER, "Cannot create a game.", 1, TextFormatter::RED);
					}
				}
			}
			else
			{
				sendPublicMessage(demoState, mCurrentInput, MessageType::EITHER);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void TictactoeScene::handleInputYourTurn(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, TicTacStep::OPPONENT_LEAVE_SERVER_CONFIRM)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandOutput;
			if (processCommand(demoState, mCurrentInput, command, commandOutput))
			{
				if (command.compare(PLAY_TURN_COMMAND) == 0)
				{
					const std::pair<int, int> boardLocation = getIndexOnBoard((a3_KeyboardKey)commandOutput[0]);
					gs_tictactoe_space_state result = gs_tictactoe_getSpaceState(mTictacBoard, boardLocation.first, boardLocation.second);

					if (result != gs_tictactoe_space_open)
					{
						addToChatList(MessageType::PLAYER, "Invalid move! Go again.", 1, TextFormatter::RED);
					}
					else
					{
						gs_tictactoe_setSpaceState(mTictacBoard, mPlayerSignature, boardLocation.first, boardLocation.second);
						finishTurn(demoState);
					}
				}
			}
			else
			{
				sendPublicMessage(demoState, mCurrentInput, MessageType::PLAYER);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void TictactoeScene::handleInputOpponentsTurn(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, TicTacStep::OPPONENT_LEAVE_SERVER_CONFIRM)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandOutput;
			if (!processCommand(demoState, mCurrentInput, command, commandOutput))
			{
				sendPublicMessage(demoState, mCurrentInput, MessageType::PLAYER);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void TictactoeScene::handleInputSpectator(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, TicTacStep::SPECTATOR_LEAVE_SERVER_CONFIRM)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			std::string command, commandResult;
			if (!processCommand(demoState, mCurrentInput, command, commandResult))
			{
				sendPublicMessage(demoState, mCurrentInput, MessageType::SPECTOR);
			}

			mCurrentInput.clear();
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void TictactoeScene::networkReceive(const a3_DemoState* demoState)
{
	Scene::networkReceive(demoState);
}

void TictactoeScene::update(const a3_DemoState* demoState)
{

}

void TictactoeScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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

	row = std::string(std::string(" ") + getXOSpace(2, 0) + std::string(" | ") + getXOSpace(2, 1) + std::string(" | ") + getXOSpace(2, 2));
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
		else if (iter->type == MessageType::PLAYER && mPlayerType == PlayerType::PLAYER1 || mPlayerType == PlayerType::PLAYER2)
		{
			formatter.drawText(demoState, iter->text, iter->color);
		}
		else if (iter->type == MessageType::SPECTOR && mPlayerType == PlayerType::SPECTATOR)
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
	if (mTictacBoard[i][j] == gs_tictactoe_space_o)
		return 'O';
	else if (mTictacBoard[i][j] == gs_tictactoe_space_x)
		return 'X';
	else if (mTictacBoard[i][j] == gs_tictactoe_space_open)
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
		addToChatList(MessageType::EITHER, "That player can't play by himself!", 1, TextFormatter::RED);
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
				mCurrentStep = TicTacStep::OPPONENTS_TURN;
			}

			SetupTictacGame setupTictacPacket = SetupTictacGame(
				player1Id, Host::spInstance->mpConnectedUsers.at(player1Id)->getUsername(),
				player2Id, Host::spInstance->mpConnectedUsers.at(player2Id)->getUsername());

			Host::spInstance->broadcastPacket((const char*)(&setupTictacPacket), sizeof(SetupTictacGame));

			//Setup game for host
			mPlayer1Id = player1Id;
			mPlayer2Id = player2Id;
			mPlayer1Username = player1;
			mPlayer2Username = player2;
			mChatLog.clear();

			if (Host::spInstance->mpHost->getUserId() == mPlayer1Id)
			{
				mPlayerType = PlayerType::PLAYER1;
				//You are player 1!
				addToChatList(MessageType::PLAYER, "You are player 1! Congrats! - X", 2, TextFormatter::GREEN);
				addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::WHITE);
				mCurrentStep = TicTacStep::YOUR_TURN;
				mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_x;
			}
			else if (Host::spInstance->mpHost->getUserId() == mPlayer2Id)
			{
				mPlayerType = PlayerType::PLAYER2;
				addToChatList(MessageType::PLAYER, "You are player 2! Congrats! - O", 2, TextFormatter::GREEN);
				addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::WHITE);
				mCurrentStep = TicTacStep::OPPONENTS_TURN;
				mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_o;
			}
			else
			{
				mPlayerType = PlayerType::SPECTATOR;
				addToChatList(MessageType::SPECTOR, "You are a spectator :)", 2, TextFormatter::WHITE);
				mCurrentStep = TicTacStep::SPECTATOR;
			}

			return true;
		}
	}
	
	return false;
}

//void TictactoeScene::broadcastMessage(const a3_DemoState* demoState, std::string message)
//{
//	MessageType type = MessageType::EITHER;
//
//	switch (mPlayer)
//	{
//	case PlayerType::PLAYER1:
//	case PlayerType::PLAYER2:
//		type = MessageType::PLAYER;
//		break;
//	case PlayerType::SPECTATOR:
//		type = MessageType::SPECTOR;
//		break;
//	}
//
//	if (Client::isHost())
//	{
//		DeliverPublicMessagePacket deliverPacket = DeliverPublicMessagePacket(Host::spInstance->mpHost->getUserId(), message, type);
//		Host::spInstance->broadcastPacket((const char*)(&deliverPacket), sizeof(DeliverPublicMessagePacket));
//	}
//	else
//	{
//		Client::spInstance->sendPublicMessage(message, type);
//	}
//}
