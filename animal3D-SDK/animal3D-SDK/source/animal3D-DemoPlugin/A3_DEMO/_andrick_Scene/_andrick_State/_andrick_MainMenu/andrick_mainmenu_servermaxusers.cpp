#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_servermaxusers.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_serverboids.h>
#include <A3_DEMO/_andrick_Event/andrick_eventSystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

MainMenuServerMaxUsers::MainMenuServerMaxUsers(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS, DARK_GREY),
	mNormalText("How many connections do you want to allow on the server?"),
	mErrorText("Please enter a valid number!"),
	mOutputText(mNormalText),
	mNormalTextColor(WHITE),
	mErrorTextColor(RED),
	mOutputTextColor(mNormalTextColor)
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to Main Menu]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
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
			if (gDemoState->mpServer)
				gDemoState->mpServer->setMaxUsers(maxUsers);
			mpParentScene->switchToState(SceneId::SERVER_BOIDS, (SceneStateId)ServerBoidsScene::ServerBoidsStateId::CONTROL_PANEL);
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
	renderEscapeOption();

	gTextFormatter.setLine(1);
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
	mOutputText = mNormalText;
	mOutputTextColor = mNormalTextColor;
}