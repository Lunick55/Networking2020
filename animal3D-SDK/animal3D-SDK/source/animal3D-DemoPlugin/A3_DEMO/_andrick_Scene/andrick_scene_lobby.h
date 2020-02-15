#ifndef ANDRICK_SCENE_LOBBY_H_
#define ANDRICK_SCENE_LOBBY_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class LobbyScene : public Scene
{
public:
	const enum class LobbySceneStateId : char
	{
		INVALID_STATE = (char)SceneStateId::INVALID_STATE,
		CHATROOM
	};

	LobbyScene();
	virtual ~LobbyScene() = default;

protected:
	//virtual void input() override;
	//virtual void processIncomingEvents() override;
	//virtual void update() override;
	//virtual void packageOutgoingEvents() override;
	//virtual void render() override;

private:
	
};

#endif