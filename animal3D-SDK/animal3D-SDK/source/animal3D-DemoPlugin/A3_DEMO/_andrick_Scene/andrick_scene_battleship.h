#ifndef ANDRICK_SCENE_BATTLESHIP_H_
#define ANDRICK_SCENE_BATTLESHIP_H_

#include "andrick_scene.h"
#include "../a3_Networking/a3_Networking_gs_battleship.h"
#include "../_andrick_Network/andrick_host.h"

class BattleShipScene : public Scene
{
public:
	BattleShipScene();
	virtual ~BattleShipScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:
	bool setupPlayers(std::string player1, std::string player2);

	gs_battleship mGame;

	enum class BattleStep : a3byte
	{
		EXIT = -1,
		SELECT_PLAYERS,
		YOUR_TURN,		//Currently your turn to play
		NOT_YOUR_TURN,	//When it's the other person's turn, this player can chat
		START_GAME,
		RESET
	};

	enum class PlayerType : a3byte
	{
		PLAYER1,
		PLAYER2,
		SPECTATOR
	};

	BattleStep mCurrentStep;
	PlayerType mPlayer;

};

#endif