#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class Scene
{
	friend class SceneManager;

public:
	explicit Scene(const SceneId id);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	void setDefaultState(std::shared_ptr<SceneState> defaultState);
	void initSceneState(std::shared_ptr<SceneState> newState);
	void switchToState(const SceneId sceneId, const SceneStateId stateId);

	const SceneId getId() const;

protected:
	const SceneId mID;
	std::map<SceneStateId, std::shared_ptr<SceneState>> mSceneStateMap;

	std::shared_ptr<SceneState> mpDefaultState;
	std::shared_ptr<SceneState> mpCurrentState;

	virtual void enteringScene(const SceneStateId state);
	virtual void input();
	virtual void processIncomingEvents() = 0;
	virtual void update();
	virtual void packageOutgoingEvents() = 0;
	virtual void render();
	virtual void exitingScene();

	//Have a message convenience class
	void sendPublicMessage(const std::string& message, MessageType type);
};

#endif