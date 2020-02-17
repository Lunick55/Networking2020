//#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
//#include "andrick_scene_manager.h"
//#include "andrick_scene_selectrole.h"
//#include "andrick_scene_lobby.h"

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_lobby.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

SceneManager::SceneManager(std::shared_ptr<Scene> defaultScene) :
	mpDefaultScene(defaultScene),
	mpCurrentScene(mpDefaultScene)
{
	assert(mpDefaultScene != nullptr);
	initScene(mpDefaultScene);
}

SceneManager::~SceneManager()
{
	//gEventSystem.removeListener(std::shared_ptr<SceneManager>(this));
	mpCurrentScene = nullptr;
	mSceneMap.clear();
}

void SceneManager::initScene(std::shared_ptr<Scene> newScene)
{
	assert(newScene != nullptr);
	mSceneMap.insert({ newScene->mID, newScene });
}

void SceneManager::switchToScene(SceneId sceneId, SceneStateId stateId)
{
	mpCurrentScene->exitingScene();

	auto iter = mSceneMap.find(sceneId);
	if (iter != mSceneMap.end())
	{
		mpCurrentScene = iter->second;
	}
	else
	{
		std::cout << "Could not find scene with id: " << std::to_string((unsigned char)sceneId) << std::endl;
		mpCurrentScene = mpDefaultScene;
	}

	mpCurrentScene->enteringScene(stateId);
}

void SceneManager::input()
{
	mpCurrentScene->input();
	gDemoState->newInput.clear();
}

void SceneManager::processIncomingEvent(std::shared_ptr<struct Event> evnt)
{
	mpCurrentScene->processIncomingEvent(evnt);
}

void SceneManager::update()
{
	mpCurrentScene->update();
}

void SceneManager::queueOutgoingEvents()
{
	mpCurrentScene->queueOutgoingEvents();
}

void SceneManager::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	mpCurrentScene->render();
}