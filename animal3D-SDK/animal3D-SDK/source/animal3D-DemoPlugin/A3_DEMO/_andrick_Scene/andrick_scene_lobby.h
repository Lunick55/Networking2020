#ifndef ANDRICK_SCENE_LOBBY_H_
#define ANDRICK_SCENE_LOBBY_H_

#include "andrick_scene.h"

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:
	enum class GameType : a3byte
	{
		NONE,
		TICTAC,
		BATTLESHIP
	};

	enum class LobbyStep : a3byte
	{
		LEAVE_SERVER = -1,
		ATTEMPTING_TO_CONNECT,
		CONNECTION_FAILED,
		UNKNOWN_ERROR,
		CHOOSE_GAME
	};

	LobbyStep mCurrentStep;
	GameType mSelectedGame;
};

#endif