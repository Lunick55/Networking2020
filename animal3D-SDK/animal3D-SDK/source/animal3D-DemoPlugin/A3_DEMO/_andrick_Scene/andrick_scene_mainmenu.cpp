#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_mainmenu.h"

MainMenuScene::MainMenuScene() :
	Scene(SceneId::MainMenu)
{

}

void MainMenuScene::input(const a3_DemoState* demoState)
{
	if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
	{
		demoState->sceneManager->switchToScene(SceneId::Tictactoe);
	}
}

void MainMenuScene::networkReceive(const a3_DemoState* demoState)
{

}

void MainMenuScene::update(const a3_DemoState* demoState)
{

}

void MainMenuScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.0f, 0.5f, 2.0f, 1.0f);
	a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%+.3lf", (a3f32)demoState->renderTimer->totalTime);
}


void MainMenuScene::networkSend(const a3_DemoState* demoState)
{

}