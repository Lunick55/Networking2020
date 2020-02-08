#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_selectrole.h"

SelectRoleScene::SelectRoleScene() :
	Scene(SceneId::SelectRole)
{

}

void SelectRoleScene::input(const a3_DemoState* demoState)
{
	if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0)
	{
		demoState->sceneManager->switchToScene(SceneId::Tictactoe);
	}
}

void SelectRoleScene::networkReceive(const a3_DemoState* demoState)
{

}

void SelectRoleScene::update(const a3_DemoState* demoState)
{

}

void SelectRoleScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.0f, 0.5f, 2.0f, 1.0f);

	//Is there a way to set text size?
	//Is there a way to center text?
	//a3textDraw(demoState->text, -0.9f, 0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "Welcome!");
	
	TextFormatter::get().setLine(0);

	for (int i = 0; i < 10; ++i)
	{
		TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE);
		TextFormatter::get().newLine();
	}

	TextFormatter::get().setLine(0);

	for (int i = 0; i < 10; ++i)
	{
		TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE, TextFormatter::TextAlign::RIGHT);
		TextFormatter::get().newLine();
	}

	TextFormatter::get().setLine(0);

	for (int i = 0; i < 10; ++i)
	{
		TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
		TextFormatter::get().newLine();
	}

	for (int i = 0; i < 10; ++i)
	{
		a3vec3 randPos;
		randPos.x = ((rand() % 100) / 50.0f) - 1.0f;
		randPos.y = ((rand() % 100) / 50.0f) - 1.0f;
		randPos.z = -1.0f;
		TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::createColor(1.0f, 0.0, 0.0, 1.0f), TextFormatter::TextAlign::UNALIGN, randPos);
	}

	//a3textDraw(demoState->text, -0.9f, 0.3f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "Would you like to host or join a server?");



	//a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%+.3lf", (a3f32)demoState->renderTimer->totalTime);
	//a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%+.3lf", (a3f32)demoState->renderTimer->totalTime);
	//a3textDraw(demoState->text, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, "%s", testScene.helloWorld.c_str());
}


void SelectRoleScene::networkSend(const a3_DemoState* demoState)
{

}