#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

class Scene
{
	friend class SceneManager;

public:
	const enum class SceneStateId : char
	{
		INVALID_STATE = -1,
		NEXT_SCENE_STATE
	};

	explicit Scene(const SceneId id, std::shared_ptr<class SceneState> defaultState);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	void initSceneState(std::shared_ptr<class SceneState> newState);

	const SceneId getId() const;

protected:
	const SceneId mID;
	std::map<SceneStateId, std::shared_ptr<class SceneState>> mSceneStateMap;

	std::shared_ptr<class SceneState> mpDefaultState;
	std::shared_ptr<class SceneState> mpCurrentState;

	virtual void enteringScene();
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