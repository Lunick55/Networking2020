#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_tictactoe.h"

TictactoeScene::TictactoeScene() :
	Scene(SceneId::Tictactoe)
{

}

void TictactoeScene::input(const a3_DemoState* demoState)
{
	if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
	{
		demoState->sceneManager->switchToScene(SceneId::SelectRole);
	}
}

void TictactoeScene::networkReceive(const a3_DemoState* demoState)
{

}

void TictactoeScene::update(const a3_DemoState* demoState)
{

}

void TictactoeScene::render(const a3_DemoState* demoState)
{
	glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
	a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%i", (a3i32)getId());
}


void TictactoeScene::networkSend(const a3_DemoState* demoState)
{

}