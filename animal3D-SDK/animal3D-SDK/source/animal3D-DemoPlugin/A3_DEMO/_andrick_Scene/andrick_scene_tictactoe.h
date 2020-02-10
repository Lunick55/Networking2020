#ifndef ANDRICK_SCENE_TICTACTOE_H_
#define ANDRICK_SCENE_TICTACTOE_H_

#include "andrick_scene.h"
#include "../a3_Networking/a3_Networking_gs_tictactoe.h"
#include <map>

class TictactoeScene : public Scene
{
	friend class Client;
	friend class Host;

public:
	TictactoeScene();
	virtual ~TictactoeScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:
	bool setupPlayers(std::string player1, std::string player2);
	void broadcastMessage(const a3_DemoState* demoState, std::string message);

	gs_tictactoe mGame;
	enum class TicTacStep : a3byte
	{
		EXIT = -1,
		SELECT_PLAYERS,
		YOUR_TURN,		//Currently your turn to play
		NOT_YOUR_TURN,	//When it's the other person's turn, this player can chat
		SPECTATING,
		RESET
	};

	enum class PlayerType : a3byte
	{
		PLAYER1,
		PLAYER2,
		SPECTATOR
	};

	TicTacStep mCurrentStep;
	PlayerType mPlayer;
	gs_tictactoe_space_state mPlayerSignature;

	a3vec3 mBoardPosition;
	a3vec3 mPlayer1Position;
	a3vec3 mPlayer2Position;
	float mBoardStartY;

	UserId mPlayer1Id;
	UserId mPlayer2Id;
	std::string mPlayer1Username;
	std::string mPlayer2Username;

	char getXOSpace(int i, int j);
};

#endif