#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_scenestate_menuoptions.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>

SceneStateMenuOptions::SceneStateMenuOptions(std::weak_ptr<class Scene> parentScene) :
	SceneState(parentScene, (Scene::SceneStateId)MainMenuScene::MenuSceneStateId::MAIN_MENU,
		AndrickColors::LIGHT_BLUE)
{
	//mMenuOptions.push_back(MenuOption("Welcome!", SceneId::MainMenu, (Scene::SceneStateId)MainMenuScene::MenuSceneStateId::MAIN_MENU));
}

void SceneStateMenuOptions::enteringState()
{
	SceneState::enteringState();
}

void SceneStateMenuOptions::processInput()
{
	SceneState::processInput();
}

void SceneStateMenuOptions::update()
{
	SceneState::update();
}

void SceneStateMenuOptions::render()
{
	SceneState::render();

	gTextFormatter.setLine(0);
	gTextFormatter.drawText("Welcome to the Main Menu!", AndrickColors::WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
}

void SceneStateMenuOptions::exitingState()
{
	SceneState::exitingState();
}