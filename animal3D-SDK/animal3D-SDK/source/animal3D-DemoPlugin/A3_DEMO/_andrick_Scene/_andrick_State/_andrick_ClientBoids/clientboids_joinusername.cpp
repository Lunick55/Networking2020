#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_joinusername.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>

ClientBoidsJoinUsername::ClientBoidsJoinUsername(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::JOIN_USERNAME, DARK_GREY),
	mNormalText("Please enter a username you'd like to use."),
	mInvalidUsernameText("Please enter a username > 0 and < " + std::to_string(sMAX_USERNAME_LENGTH) + " chars and with no spaces or commas."),
	mUsernameTakenText("That username is already taken. Please pick a different one."),
	mOutputText(mNormalText),
	mNormalTextColor(WHITE),
	mErrorTextColor(RED),
	mOutputTextColor(mNormalTextColor)
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to main menu]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void ClientBoidsJoinUsername::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsJoinUsername::processInput()
{
	SceneState::processInput();

	if (mpInputHandler->isKeyPressed(a3key_enter))
	{
		if (!mpInputHandler->validateUsername(mpInputHandler->getCurrentInput()))
		{
			mOutputText = mInvalidUsernameText;
			mOutputTextColor = mErrorTextColor;
		}
		else
		{
			//if (/*Username is already taken*/0)
			//{
			//	mOutputText = mUsernameTakenText;
			//	mOutputTextColor = mErrorTextColor;
			//}
			//else
			//{
			gDemoState->mpClient->setUsername(mpInputHandler->getCurrentInput());
			mpParentScene->switchToState(SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONNECT_LOAD_STATE);
			//}
		}

		mpInputHandler->clearCurrentInput();
	}
}

void ClientBoidsJoinUsername::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ClientBoidsJoinUsername::update()
{

}

void ClientBoidsJoinUsername::queueOutgoingEvents()
{

}

void ClientBoidsJoinUsername::render()
{
	gTextFormatter.setLine(1);
	gTextFormatter.setAlignment(TextAlign::CENTER_X);

	gTextFormatter.drawText("Welcome to the Client Boids Scene!", WHITE);
	gTextFormatter.offsetLine(6);
	renderMenuOptions(WHITE, TextAlign::CENTER_X);
	gTextFormatter.offsetLine(2);
	gTextFormatter.drawText(mOutputText, mOutputTextColor);

	gTextFormatter.offsetLine(5);
	gTextFormatter.drawText(mpInputHandler->getCurrentInput(), mNormalTextColor);
}

void ClientBoidsJoinUsername::exitingState()
{
	SceneState::exitingState();
	mOutputText = mNormalText;
	mOutputTextColor = mNormalTextColor;
}
