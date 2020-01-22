#include "SceneManager.h"

//Do more in the future here if we need to.
void SceneManager::initSceneManager()
{
	SceneManager& sceneManager = instance();
}

SceneManager::SceneManager() :
	mIsRunning(false),
	mpMenuScene(std::make_shared<MenuScene>()),
	mpJoinChatRoomScene(std::make_shared<JoinChatRoomScene>()),
	mpChatRoomScene(std::make_shared<ChatRoomScene>()),
	mpCreateChatRoomScene(std::make_shared<CreateChatRoomScene>())
{
	mpScenes.insert({ mpMenuScene->getId(), mpMenuScene });
	mpScenes.insert({ mpChatRoomScene->getId(), mpChatRoomScene });
	mpScenes.insert({ mpJoinChatRoomScene->getId(), mpJoinChatRoomScene });
	mpScenes.insert({ mpCreateChatRoomScene->getId(), mpCreateChatRoomScene});
}

void SceneManager::start()
{
	SceneManager& sceneManager = instance();
	if (!sceneManager.mIsRunning)
	{
		sceneManager.mIsRunning = true;
		sceneManager.loop();
	}
}

void SceneManager::stop()
{
	SceneManager& sceneManager = instance();
	sceneManager.mIsRunning = false;
}

void::SceneManager::swapScene(SceneId sceneId)
{
	Scene::clearScreen();

	auto iter = mpScenes.find(sceneId);
	if (iter != mpScenes.end())
	{
		mpCurrentScene = iter->second;
		mpCurrentScene->enteringScene();
	}
	else
	{
		std::cout << "UH NOT READY/";
	}
}

void SceneManager::switchScene(SceneId sceneId)
{
	SceneManager& m = instance();
	m.swapScene(sceneId);
}

void SceneManager::loop()
{
	switchScene(SceneId::MAIN_MENU);

	while (mIsRunning)
	{
		mpCurrentScene->update();
		getInput();
		mpCurrentScene->render();
	}
}

void SceneManager::getInput()
{
	//While a key is pressed.
	while (_kbhit())
	{
		//Get the current charater that was pressed.
		char input = _getch();
		mpCurrentScene->handleInput(input);
	}
}