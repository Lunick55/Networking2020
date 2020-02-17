#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

MainMenuScene::MainMenuScene() :
	Scene(SceneId::MAIN_MENU)
{
	//init scene states
	setDefaultState(std::make_shared<MainMenuTitle>(*this));
	initSceneState(std::make_shared<MainMenuServerMaxUsers>(*this));
}
