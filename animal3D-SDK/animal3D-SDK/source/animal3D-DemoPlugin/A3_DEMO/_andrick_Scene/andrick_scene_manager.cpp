#include "andrick_scene_manager.h"

SceneManager::SceneManager() :
	mpMainMenu(std::make_shared<MainMenuScene>()),
	mpCurrentScene(mpMainMenu)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::switchToScene(enum class SceneId id)
{
	switch (id)
	{
	case SceneId::MainMenu:
		mpCurrentScene = mpMainMenu;
		break;
	default:
		break;
	}
}

void SceneManager::update()
{

}

void SceneManager::render()
{

}

void SceneManager::input()
{

}