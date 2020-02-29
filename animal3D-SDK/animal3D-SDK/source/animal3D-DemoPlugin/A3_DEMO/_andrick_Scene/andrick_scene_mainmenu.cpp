#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>

MainMenuScene::MainMenuScene() :
	Scene(SceneId::MAIN_MENU)
{
	//init scene states
	std::shared_ptr<MainMenuScene> instance = std::shared_ptr<MainMenuScene>(this);
	setDefaultState(std::make_shared<MainMenuTitle>(instance));
	initSceneState(std::make_shared<MainMenuServerMaxUsers>(instance));
}

void MainMenuScene::enteringScene(SceneStateId id)
{
	Scene::enteringScene(id);

	if (gDemoState->mpServer)
		gDemoState->mpServer->setMaxUsers(sMAX_USERS);
}