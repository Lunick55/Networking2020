#ifndef SCENE_STATE_H_
#define SCENE_STATE_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class SceneState
{
	friend class Scene;

public:
	SceneState(std::weak_ptr<Scene> parentScene, Scene::SceneStateId id, Color backgroundColor = AndrickColors::DARK_GREY);
	virtual ~SceneState() = default;

	SceneState(const SceneState& scene) = delete;
	SceneState& operator=(const SceneState& scene) = delete;

	virtual void enteringState();
	virtual void processInput();
	virtual void update() = 0;
	virtual void render();
	virtual void exitingState();

	const Scene::SceneStateId getId() const;

protected:
	const std::weak_ptr<Scene> mpParentScene;
	const Scene::SceneStateId mID;
	std::shared_ptr<class SceneInputHandler> mpInputHandler;
	Color mBackgroundColor;
};

#endif