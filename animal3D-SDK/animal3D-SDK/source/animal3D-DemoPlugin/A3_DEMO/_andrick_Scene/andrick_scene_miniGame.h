#ifndef ANDRICK_SCENE_MINIGAME_H_
#define ANDRICK_SCENE_MINIGAME_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class MiniGameScene : public Scene
{
public:
	const enum class MiniGameSceneStateId : char
	{
		INVALID_STATE = (char)SceneStateId::INVALID_STATE,
		MINIGAME
	};

	MiniGameScene();
	virtual ~MiniGameScene() = default;

private:

};

#endif