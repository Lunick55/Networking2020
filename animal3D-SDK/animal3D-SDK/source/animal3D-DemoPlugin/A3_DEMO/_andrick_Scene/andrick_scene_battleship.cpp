#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_battleship.h"

BattleShipScene::BattleShipScene() :
	Scene(SceneId::Battleship)
{

}

void BattleShipScene::enteringScene(const a3_DemoState* demoState)
{

}


void BattleShipScene::input(a3_DemoState* demoState)
{
	if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
	{
		demoState->mpSceneManager->switchToScene(demoState, SceneId::SelectRole);
	}
}

void BattleShipScene::networkReceive(const a3_DemoState* demoState)
{

}

void BattleShipScene::update(const a3_DemoState* demoState)
{

}

void BattleShipScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.4f, 1.0f, 0.2f, 1.0f);
	
	TextFormatter& formatter = TextFormatter::get();

	formatter.setLine(0);
	formatter.drawText(demoState, "Battleship Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	formatter.offsetLine(2);
}


void BattleShipScene::networkSend(const a3_DemoState* demoState)
{

}