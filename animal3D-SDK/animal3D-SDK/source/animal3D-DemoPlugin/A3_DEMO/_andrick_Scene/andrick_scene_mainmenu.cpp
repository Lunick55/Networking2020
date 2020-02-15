#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>

MainMenuScene::MainMenuScene() :
	Scene(SceneId::MAIN_MENU)
{
	//init scene states
	std::shared_ptr<MainMenuScene> scene = std::shared_ptr<MainMenuScene>(this);
	setDefaultState(std::make_shared<MainMenuTitle>(scene));
	initSceneState(std::make_shared<MainMenuServerMaxUsers>(scene));
}

void MainMenuScene::processIncomingEvents()
{
	
}

void MainMenuScene::packageOutgoingEvents()
{

}