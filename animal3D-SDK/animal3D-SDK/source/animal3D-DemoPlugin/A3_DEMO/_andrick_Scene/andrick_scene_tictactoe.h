#ifndef ANDRICK_SCENE_TICTACTOE_H_
#define ANDRICK_SCENE_TICTACTOE_H_

#include "andrick_scene.h"
#include "../a3_Networking/a3_Networking_gs_tictactoe.h"

class TictactoeScene : public Scene
{
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

	gs_tictactoe mGame;
	enum class TicTacStep : a3byte
	{
		EXIT = -1,
		SELECT_PLAYERS,
		PLAYGAME,
		RESET,

		PLAYER1,
		PLAYER2,
		SPECTATOR
	};

	TicTacStep mCurrentStep, mPlayer;
	gs_tictactoe_space_state mSpaceState;
};

#endif