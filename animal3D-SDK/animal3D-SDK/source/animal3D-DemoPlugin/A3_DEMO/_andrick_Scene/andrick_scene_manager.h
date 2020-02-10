#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include "andrick_scene.h"

class SelectRoleScene;
class LobbyScene; 
class TictactoeScene;

class SceneManager
{
	friend class Host;
	friend class Client;

public:
	SceneManager();
	~SceneManager();

	void switchToScene(const a3_DemoState* demoState, enum class SceneId id);

	void input(a3_DemoState* demoState);
	void networkReceive(const a3_DemoState* demoState);
	void update(const a3_DemoState* demoState);
	void networkSend(const a3_DemoState* demoState);
	void render(const a3_DemoState* demoState);

private:
	Scene* mpCurrentScene;
	SelectRoleScene* mpSelectRole;
	LobbyScene* mpLobby;
	TictactoeScene* mpTictactoe;
};

#endif