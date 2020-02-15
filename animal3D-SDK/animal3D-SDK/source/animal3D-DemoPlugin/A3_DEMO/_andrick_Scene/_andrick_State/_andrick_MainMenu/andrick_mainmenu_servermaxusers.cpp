#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

MainMenuServerMaxUsers::MainMenuServerMaxUsers(std::shared_ptr<class Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS,
		DARK_GREY)
{
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press ESC to go back to the main menu.", SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
	//mMenuOptions.push_back(MenuOption(a3key_2, "Press 2 to join a server.", SceneId::MainMenu, (SceneStateId)MainMenuScene::MenuSceneStateId::CLIENT_ENTER_USERNAME));
}

void MainMenuServerMaxUsers::enteringState()
{
	SceneState::enteringState();
}

void MainMenuServerMaxUsers::processInput()
{
	SceneState::processInput();
}

void MainMenuServerMaxUsers::update()
{

}

void MainMenuServerMaxUsers::render()
{
	SceneState::render();

	gTextFormatter.setLine(0);
	gTextFormatter.drawText("Welcome to the Main Menu!", WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(6);

	renderMenuOptions(WHITE, TextAlign::CENTER_X);
}

void MainMenuServerMaxUsers::exitingState()
{
	SceneState::exitingState();
}