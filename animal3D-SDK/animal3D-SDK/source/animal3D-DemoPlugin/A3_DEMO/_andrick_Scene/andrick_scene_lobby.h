#ifndef ANDRICK_SCENE_LOBBY_H_
#define ANDRICK_SCENE_LOBBY_H_

#include "andrick_scene.h"

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene() = default;

protected:
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

	GameType mSelectedGame;
};

#endif