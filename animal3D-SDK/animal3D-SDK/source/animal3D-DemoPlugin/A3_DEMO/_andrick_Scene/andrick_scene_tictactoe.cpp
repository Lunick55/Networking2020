#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_tictactoe.h"
#include "../_andrick_Network/andrick_host.h"

TictactoeScene::TictactoeScene() :
	Scene(SceneId::Tictactoe)
{
	gs_tictactoe_reset(mGame);
}

void TictactoeScene::enteringScene(const a3_DemoState* demoState)
{
	mCurrentStep = TicTacStep::SELECT_PLAYERS;
	mPlayer = TicTacStep::RESET; //for now
}


void TictactoeScene::input(a3_DemoState* demoState)
{
	if(mCurrentStep == TicTacStep::SELECT_PLAYERS)
	{
		//Input Players names
	}
	else if(mCurrentStep == TicTacStep::RESET)
	{

	}
	else if(mCurrentStep == TicTacStep::EXIT)
	{

	}
	else if(mCurrentStep == TicTacStep::PLAYER1 || mCurrentStep == TicTacStep::PLAYER2)
	{
		if (mPlayer != mCurrentStep || mPlayer == TicTacStep::SPECTATOR)
		{
			//can't play, only watch
			return;
		}

		if (mPlayer == TicTacStep::PLAYER1)
		{
			mSpaceState = gs_tictactoe_space_state::gs_tictactoe_space_x;
		}
		else if (mPlayer == TicTacStep::PLAYER2)
		{
			mSpaceState = gs_tictactoe_space_state::gs_tictactoe_space_o;
		}

		if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
		{
			//demoState->mpSceneManager->switchToScene(demoState, SceneId::SelectRole);
		}
		//TODO: interpret input?
		if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad1) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 0, 0);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad2) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 1, 0);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad3) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 2, 0);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad4) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 0, 1);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad5) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 1, 1);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad6) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 2, 1);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad7) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 0, 2);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad8) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 1, 2);
		}
		else if (a3keyboardIsChanged(demoState->keyboard, a3key_numpad9) > 0)
		{
			gs_tictactoe_setSpaceState(mGame, mSpaceState, 2, 2);
		}
		else
		{
			return;
		}

		if (mPlayer == TicTacStep::PLAYER1)
		{
			mCurrentStep = TicTacStep::PLAYER2;
			//TODO: send the packet to everyone containing gameState, and whoseTurn or whatever
		}
		else if (mPlayer == TicTacStep::PLAYER2)
		{
			mCurrentStep = TicTacStep::PLAYER1;
			//TODO: send the packet to everyone containing gameState, and whoseTurn or whatever
		}
	
	}
}

void TictactoeScene::networkReceive(const a3_DemoState* demoState)
{
	//Incoming packets to server from client.
	for (Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive(); Client::spInstance->mpPacket; Client::spInstance->mpPeer->DeallocatePacket(Client::spInstance->mpPacket), Client::spInstance->mpPacket = Client::spInstance->mpPeer->Receive())
	{
		switch (Client::spInstance->mpPacket->data[0])
		{
			case PacketEventId::UPDATE_TICTAC_STATE :
			{

			}
		}
	}
}

void TictactoeScene::update(const a3_DemoState* demoState)
{

}

void TictactoeScene::render(const a3_DemoState* demoState)
{
	//glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
	//a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%i", (a3i32)getId());
	glClearColor(1.0f, 0.5f, 0.2f, 1.0f);

	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText(demoState, "Tictactoe Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	formatter.offsetLine(2);
}


void TictactoeScene::networkSend(const a3_DemoState* demoState)
{

}

bool TictactoeScene::setupPlayers(std::string player1, std::string player2)
{
	/*
	//maybe make sure player1 and 2 arent the same?

	if(player1 == me)
	{
		mPlayer = PLAYER1;
	}
	else
	{
		SendMessageToTheRealPlayer1(theDude, PLAYER1);
	}
	if(player2 == me)
	{
		mPlayer = PLAYER2;
	}
	else
	{
		SendMessageToTheRealPlayer2(theDude, PLAYER2);
	}

	BroadCastToAll(you are spectators!);
	
	*/
	return false;
}