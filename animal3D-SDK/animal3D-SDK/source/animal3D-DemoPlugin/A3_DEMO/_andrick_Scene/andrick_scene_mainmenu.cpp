#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_scenestate_menuoptions.h>

MainMenuScene::MainMenuScene() :
	Scene(SceneId::MainMenu, 
		std::make_shared<SceneStateMenuOptions>(this))
{
	//init scene states
}

void MainMenuScene::processIncomingEvents()
{
	
}

void MainMenuScene::packageOutgoingEvents()
{

}