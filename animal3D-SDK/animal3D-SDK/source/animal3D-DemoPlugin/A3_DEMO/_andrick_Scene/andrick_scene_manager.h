#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include <memory>
#include "andrick_scene.h"

class MainMenuScene;
class TictactoeScene;

struct a3_DemoState;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void switchToScene(enum class SceneId id);

	void input(const a3_DemoState* demoState);
	void networkReceive(const a3_DemoState* demoState);
	void update(const a3_DemoState* demoState);
	void networkSend(const a3_DemoState* demoState);
	void render(const a3_DemoState* demoState);

private:
	Scene* mpCurrentScene;
	MainMenuScene* mpMainMenu;
	TictactoeScene* mpTictactoe;
};

#endif