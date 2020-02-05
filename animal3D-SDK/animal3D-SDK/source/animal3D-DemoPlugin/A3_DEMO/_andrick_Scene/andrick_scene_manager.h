#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include <memory>
#include "andrick_scene_mainmenu.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void switchToScene(enum class SceneId id);

	void update();
	void render();
	void input();

private:
	std::shared_ptr<Scene> mpCurrentScene;
	std::shared_ptr<MainMenuScene> mpMainMenu;
};

#endif