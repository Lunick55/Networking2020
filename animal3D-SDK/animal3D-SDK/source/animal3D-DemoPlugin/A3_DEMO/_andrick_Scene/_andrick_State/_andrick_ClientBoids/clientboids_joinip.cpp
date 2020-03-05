#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_joinip.h>
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

ClientBoidsJoinIP::ClientBoidsJoinIP(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::JOIN_IP, DARK_GREY),
	mNormalText("Please enter the server ip."),
	mInvalidIPText("Please enter a valid ip address!"),
	mFailedToConnectText("Failed to connect to the server."),
	mOutputText(mNormalText),
	mNormalTextColor(WHITE),
	mErrorTextColor(RED),
	mOutputTextColor(mNormalTextColor)
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to main menu]", nullptr, SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU));
}

void ClientBoidsJoinIP::enteringState()
{
	SceneState::enteringState();
}

void ClientBoidsJoinIP::processInput()
{
	SceneState::processInput();

	if (mpInputHandler->isKeyPressed(a3key_enter))
	{
		std::string ipAddress;
		if (!mpInputHandler->validateIPAddress(mpInputHandler->getCurrentInput(), ipAddress))
		{
			mOutputText = mInvalidIPText;
			mOutputTextColor = mErrorTextColor;
		}
		else
		{
			if (gDemoState->mpPacketHandler->startup(1))
			{
				if (gDemoState->mpPacketHandler->connect(ipAddress.c_str()))
				{
					std::cout << "Client spinning up..." << std::endl;
					mpParentScene->switchToState(SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONNECT_LOAD_STATE);
				}
				else
				{
					std::cout << "Failed to connect to server." << std::endl;
					mOutputText = mFailedToConnectText;
					mOutputTextColor = mErrorTextColor;
				}
			}
		}

		mpInputHandler->clearCurrentInput();
	}
}

void ClientBoidsJoinIP::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void ClientBoidsJoinIP::update()
{

}

void ClientBoidsJoinIP::queueOutgoingEvents()
{

}

void ClientBoidsJoinIP::render()
{
	SceneState::render();
	renderEscapeOption();

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

void ClientBoidsJoinIP::exitingState()
{
	SceneState::exitingState();
	mOutputText = mNormalText;
	mOutputTextColor = mNormalTextColor;
}
