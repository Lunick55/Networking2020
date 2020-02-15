#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include <GL/glew.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class SceneManager
{
public:
	SceneManager(std::shared_ptr<class Scene> defaultScene);
	~SceneManager();

	void initScene(std::shared_ptr<class Scene> newScene);

	void switchToScene(enum class SceneId id);

	void input();
	void processIncomingEvents();
	void update();
	void packageOutgoingEvents();
	void render();

private:
	std::map<SceneId, std::shared_ptr<class Scene>> mSceneMap;
	std::shared_ptr<class Scene> mpDefaultScene;
	std::shared_ptr<class Scene> mpCurrentScene;
};

#endif