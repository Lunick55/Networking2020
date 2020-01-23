#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "Common.h"
#include "Scene.h"
#include "ChatRoomScene.h"
#include <map>

class SceneManager
{
public:
	static void initSceneManager();
	static void start();
	static void stop();

	static void switchScene(SceneId sceneId);

private:
	static SceneManager& instance()
	{
		static SceneManager sInstance;
		return sInstance;
	}

	bool mIsRunning;
	std::shared_ptr<Scene> mpCurrentScene;

	std::map<SceneId, std::shared_ptr<Scene>> mpScenes;

	std::shared_ptr<MenuScene> mpMenuScene;
	std::shared_ptr<JoinChatRoomScene> mpJoinChatRoomScene;
	std::shared_ptr<ChatRoomScene> mpChatRoomScene;
	std::shared_ptr<CreateChatRoomScene> mpCreateChatRoomScene;

	SceneManager();
	~SceneManager() = default;

	void loop();
	void getInput();
	void swapScene(SceneId sceneId);
};

#endif