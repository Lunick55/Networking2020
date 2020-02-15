#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include "andrick_scene.h"

class SceneManager
{
public:
	SceneManager(std::shared_ptr<Scene> defaultScene);
	~SceneManager();

	void initScene(std::shared_ptr<Scene> newScene);

	void switchToScene(enum class SceneId id);

	void input();
	void processIncomingEvents();
	void update();
	void packageOutgoingEvents();
	void render();

private:
	std::map<SceneId, std::shared_ptr<Scene>> mSceneMap;
	std::shared_ptr<Scene> mpDefaultScene;
	std::shared_ptr<Scene> mpCurrentScene;
};

#endif