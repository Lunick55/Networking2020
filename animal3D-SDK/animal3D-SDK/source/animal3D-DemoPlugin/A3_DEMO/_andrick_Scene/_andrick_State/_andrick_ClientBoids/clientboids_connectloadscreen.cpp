#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_connectloadscreen.h>
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

ClientBoidsConnectLoadScreen::ClientBoidsConnectLoadScreen(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONNECT_LOAD_STATE, LIGHT_BLUE),
	mLoadText("Attempting to connect to Server"),
	mConnectionTimedOut("Couldn't connect to server. Timed out :("),
	mOutputText(mLoadText),
	mNormalTextColor(WHITE),
	mErrorTextColor(RED),
	mOutputTextColor(mNormalTextColor),
	mShouldUpdate(true)
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to disconnect]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void ClientBoidsConnectLoadScreen::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsConnectLoadScreen::processInput()
{
	SceneState::processInput();
}

void ClientBoidsConnectLoadScreen::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::CONNECTION_REQUEST_FAILED:
	{
		mOutputText = std::reinterpret_pointer_cast<ConnectionRequestFailedEvent>(evnt)->errorMessage;
		mOutputTextColor = mErrorTextColor;
		mShouldUpdate = false;
		break;
	}
	case EventId::CONNECTION_REQUEST_ACCEPTED:
	{
		mpParentScene->switchToState(SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CLIENT_WORLD);
		break;
	}
	default:
		break;
	}
}

void ClientBoidsConnectLoadScreen::update()
{
	if (mShouldUpdate)
	{
		mTimeoutUpdateCounter += gDemoState->renderTimer->secondsPerTick;

		if (mTimeoutUpdateCounter >= mCONNECTION_TIMEOUT_TIME)
		{
			mTimeoutUpdateCounter = 0.0f;
			mShouldUpdate = false;
			mOutputText = mConnectionTimedOut;
			mOutputTextColor = mErrorTextColor;
			return;
		}

		mDotUpdateCounter += gDemoState->renderTimer->secondsPerTick;

		if (mDotUpdateCounter >= mCHANGE_DOT_AFTER_SECONDS)
		{
			mDotUpdateCounter = 0.0f;
			if (mDotText.size() >= mMAX_DOT_LENGTH)
			{
				mDotText.clear();
			}
			else
			{
				mDotText.append(".");
			}

			mOutputText = mLoadText + mDotText;
		}
	}
}

void ClientBoidsConnectLoadScreen::queueOutgoingEvents()
{

}

void ClientBoidsConnectLoadScreen::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.drawText(mOutputText, mOutputTextColor, TextAlign::CENTER);

	gTextFormatter.setLine(5);
	renderMenuOptions(mOutputTextColor, TextAlign::LEFT);
}

void ClientBoidsConnectLoadScreen::exitingState()
{
	SceneState::exitingState();
	mOutputText = mLoadText;
	mOutputTextColor = mNormalTextColor;

	mTimeoutUpdateCounter = 0.0f;

	mDotUpdateCounter = 0.0f;
	mDotText.clear();
}
