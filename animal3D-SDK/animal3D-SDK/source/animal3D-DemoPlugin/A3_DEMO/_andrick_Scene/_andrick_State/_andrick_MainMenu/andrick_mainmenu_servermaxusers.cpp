#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>

MainMenuServerMaxUsers::MainMenuServerMaxUsers(std::shared_ptr<class Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS, DARK_GREY),
	mNormalText("How many connections do you want to allow on the server?"),
	mErrorText("Please enter a valid number!"),
	mOutputText(mNormalText),
	mNormalTextColor(WHITE),
	mErrorTextColor(RED),
	mOutputTextColor(mNormalTextColor)
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

	if (mpInputHandler->isKeyPressed(a3key_enter))
	{
		int maxUsers;
		if (!mpInputHandler->validateNumber(mpInputHandler->getCurrentInput(), maxUsers))
		{
			mOutputText = mErrorText;
			mOutputTextColor = mErrorTextColor;
		}
		else
		{
			mpParentScene->switchToState(SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU);
		}

		mpInputHandler->clearCurrentInput();
	}
}

void MainMenuServerMaxUsers::update()
{

}

void MainMenuServerMaxUsers::render()
{
	SceneState::render();
	gTextFormatter.setLine(0);
	gTextFormatter.setAlignment(TextAlign::CENTER_X);

	gTextFormatter.drawText("Welcome to the Main Menu!", WHITE);
	gTextFormatter.offsetLine(6);
	renderMenuOptions(WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText(mOutputText, mOutputTextColor);

	gTextFormatter.offsetLine(5);
	gTextFormatter.drawText(mpInputHandler->getCurrentInput(), mNormalTextColor);
}

void MainMenuServerMaxUsers::exitingState()
{
	SceneState::exitingState();
}