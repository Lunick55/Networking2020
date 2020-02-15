#ifndef SCENE_STATE_H_
#define SCENE_STATE_H_

#include <GL/glew.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class SceneState
{
	friend class Scene;

public:
	SceneState(std::shared_ptr<Scene> parentScene, SceneStateId id, Color backgroundColor = DARK_GREY);
	virtual ~SceneState() = default;

	SceneState(const SceneState& scene) = delete;
	SceneState& operator=(const SceneState& scene) = delete;

	virtual void enteringState();
	virtual void processInput();
	virtual void update() = 0;
	virtual void render();
	virtual void exitingState();

	const enum SceneStateId getId() const;

	void renderMenuOptions(Color color, TextAlign alignment, a3vec3 viewportPosition = a3vec3());

protected:
	const std::shared_ptr<Scene> mpParentScene;
	const SceneStateId mID;
	std::shared_ptr<class SceneInputHandler> mpInputHandler;
	std::vector<struct MenuOption> mMenuOptions;
	Color mBackgroundColor;
};

#endif