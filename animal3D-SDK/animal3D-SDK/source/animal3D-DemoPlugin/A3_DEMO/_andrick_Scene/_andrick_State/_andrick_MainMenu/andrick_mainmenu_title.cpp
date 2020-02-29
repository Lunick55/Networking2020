#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MainMenu/andrick_mainmenu_title.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>

MainMenuTitle::MainMenuTitle(class Scene& parentScene) :
	SceneState(parentScene, (SceneStateId)MainMenuScene::MenuSceneStateId::TITLE_MENU,
		LIGHT_BLUE)
{
	mMenuOptions.push_back(MenuOption(a3key_escape, "Press ESC to exit.", SceneId::EXIT, SceneStateId::EXIT_STATE));
	mMenuOptions.push_back(MenuOption(a3key_1, "Press 1 to host a server.", SceneId::MAIN_MENU, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MAX_USERS, initializeServer));
	mMenuOptions.push_back(MenuOption(a3key_2, "Press 2 to go to hell.", SceneId::MINIGAME, (SceneStateId)MainMenuScene::MenuSceneStateId::SERVER_MINIGAME, initializeClient));
}

void MainMenuTitle::enteringState()
{
	SceneState::enteringState();

	if (gDemoState->mpServer)
	{
		gEventSystem.removeListener(gDemoState->mpServer);
		gDemoState->mpServer.reset();
	}

	if (gDemoState->mpClient)
	{
		gEventSystem.removeListener(gDemoState->mpClient);
		gDemoState->mpClient.reset();
	}

	if (gDemoState->mpPacketHandler->disconnect())
	{
		std::cout << "Successfully disconnected." << std::endl;
		if (gDemoState->mpPacketHandler->shutdown())
		{
			std::cout << "Successfully shutdown RakNet." << std::endl;
		}
	}
}

void MainMenuTitle::processInput()
{
	SceneState::processInput();
}

void MainMenuTitle::processIncomingEvent(std::shared_ptr<Event> evnt)
{

}

void MainMenuTitle::update()
{

}

void MainMenuTitle::queueOutgoingEvents()
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
