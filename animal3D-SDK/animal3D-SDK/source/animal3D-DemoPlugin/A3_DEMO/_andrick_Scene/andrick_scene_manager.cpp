//#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
//#include "andrick_scene_manager.h"
//#include "andrick_scene_selectrole.h"
//#include "andrick_scene_lobby.h"

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

SceneManager::SceneManager(std::shared_ptr<Scene> defaultScene) :
	mpDefaultScene(defaultScene),
	mpCurrentScene(mpDefaultScene)
{
	assert(mpDefaultScene != nullptr);
}

SceneManager::~SceneManager()
{
	mpCurrentScene = nullptr;
	mSceneMap.clear();
}

void SceneManager::initScene(std::shared_ptr<Scene> newScene)
{
	assert(newScene != nullptr);
	mSceneMap.insert({ newScene->mID, newScene });
}

void SceneManager::switchToScene(enum class SceneId id)
{
	auto iter = mSceneMap.find(id);
	if (iter != mSceneMap.end())
	{
		mpCurrentScene = iter->second;
	}
	else
	{
		std::cout << "Could not find scene with id: " << std::to_string((unsigned char)id) << std::endl;
		mpCurrentScene = mpDefaultScene;
	}

	mpCurrentScene->enteringScene();
}

void SceneManager::input()
{
	mpCurrentScene->input();
	gDemoState->newInput.clear();
}

void SceneManager::processIncomingEvents()
{
	mpCurrentScene->processIncomingEvents();
}

void SceneManager::update()
{
	mpCurrentScene->update();
}

void SceneManager::packageOutgoingEvents()
{
	mpCurrentScene->packageOutgoingEvents();
}

void SceneManager::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	mpCurrentScene->render();
}