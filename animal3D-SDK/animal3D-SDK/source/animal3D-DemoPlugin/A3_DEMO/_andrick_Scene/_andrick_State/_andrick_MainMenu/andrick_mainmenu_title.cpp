#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

MainMenuTitle::MainMenuTitle(std::shared_ptr<class Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU,
		LIGHT_BLUE)
{
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press 1 to host a server.", SceneId::EXIT, SceneStateId::EXIT_STATE));
	mMenuOptions.push_back(MenuOption(a3key_1, "Press ESC to exit.", SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS));
	//mMenuOptions.push_back(MenuOption(a3key_2, "Press 2 to join a server.", SceneId::MainMenu, (SceneStateId)MainMenuScene::MenuSceneStateId::CLIENT_ENTER_USERNAME));
}

void MainMenuTitle::enteringState()
{
	SceneState::enteringState();
}

void MainMenuTitle::processInput()
{
	SceneState::processInput();
}

void MainMenuTitle::update()
{

}

void MainMenuTitle::render()
{
	SceneState::render();

	gTextFormatter.setLine(0);
	gTextFormatter.drawText("Welcome to the Main Menu!", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void MainMenuTitle::exitingState()
{
	SceneState::exitingState();
}