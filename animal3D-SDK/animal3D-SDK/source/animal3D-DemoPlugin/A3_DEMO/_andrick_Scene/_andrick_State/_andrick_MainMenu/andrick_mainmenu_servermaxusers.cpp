#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_lobby.h>

MainMenuServerMaxUsers::MainMenuServerMaxUsers(class Scene& parentScene) :
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
	//TODO: move this to after number of users decided
	a3_DemoState::a3netAddressStr const ipAddress = "127.0.0.1";
	a3ui16 const port_server = 60006;
	a3ui16 const port_client = 60005;
	a3ui16 const maxConnections_server = 16;
	a3ui16 const maxConnections_client = 1;
	if (gDemoState->a3netStartup(port_server, 0, maxConnections_server, 0) > 0)
	{
		printf("\n Server spinning up... \n");
	}
	//------------------------

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
			mParentScene.switchToState(SceneId::LOBBY, (SceneStateId)LobbyScene::LobbySceneStateId::CHATROOM);
		}

		mpInputHandler->clearCurrentInput();
	}
}

void MainMenuServerMaxUsers::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void MainMenuServerMaxUsers::update()
{

}

void MainMenuServerMaxUsers::queueOutgoingEvents()
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