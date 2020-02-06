#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_manager.h"
#include "andrick_scene_mainmenu.h"
#include "andrick_scene_tictactoe.h"

SceneManager::SceneManager() :
	mpMainMenu(new MainMenuScene()),
	mpTictactoe(new TictactoeScene())
{
	mpCurrentScene = mpMainMenu;
}

SceneManager::~SceneManager()
{
	delete mpMainMenu;
	mpMainMenu = nullptr;

	mpCurrentScene = nullptr;
}

void SceneManager::switchToScene(enum class SceneId id)
{
	switch (id)
	{
	case SceneId::MainMenu:
		mpCurrentScene = mpMainMenu;
		break;
	case SceneId::Tictactoe:
		mpCurrentScene = mpTictactoe;
		break;
	default:
		break;
	}
}

void SceneManager::input(const a3_DemoState* demoState)
{
	mpCurrentScene->input(demoState);
}

void SceneManager::networkReceive(const a3_DemoState* demoState)
{
	mpCurrentScene->networkReceive(demoState);
}

void SceneManager::update(const a3_DemoState* demoState)
{
	mpCurrentScene->update(demoState);
}

void SceneManager::networkSend(const a3_DemoState* demoState)
{
	mpCurrentScene->networkSend(demoState);
}

void SceneManager::render(const a3_DemoState* demoState)
{
	mpCurrentScene->render(demoState);
}