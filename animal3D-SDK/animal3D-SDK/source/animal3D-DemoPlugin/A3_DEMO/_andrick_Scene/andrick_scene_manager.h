#ifndef ANDRICK_SCENE_MANAGER_H_
#define ANDRICK_SCENE_MANAGER_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

class SceneManager : public EventAgnosticListener
{
public:
	SceneManager(std::shared_ptr<class Scene> defaultScene);
	~SceneManager();

	void initScene(std::shared_ptr<class Scene> newScene);

	void switchToScene(SceneId sceneId, SceneStateId stateId);

	void input();
	virtual void processIncomingEvent(std::shared_ptr<struct Event> evnt) override;
	void update();
	void queueOutgoingEvents();
	void render();

private:
	std::map<SceneId, std::shared_ptr<class Scene>> mSceneMap;
	std::shared_ptr<class Scene> mpDefaultScene;
	std::shared_ptr<class Scene> mpCurrentScene;
};

#endif