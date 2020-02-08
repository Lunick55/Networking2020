#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_manager.h"
#include "andrick_scene_selectrole.h"
#include "andrick_scene_tictactoe.h"

SceneManager::SceneManager() :
	mpSelectRole(new SelectRoleScene()),
	mpTictactoe(new TictactoeScene())
{
	mpCurrentScene = mpSelectRole;
}

SceneManager::~SceneManager()
{
	delete mpSelectRole;
	mpSelectRole = nullptr;

	delete mpTictactoe;
	mpTictactoe = nullptr;

	mpCurrentScene = nullptr;
}

void SceneManager::switchToScene(enum class SceneId id)
{
	switch (id)
	{
	case SceneId::SelectRole:
		mpCurrentScene = mpSelectRole;
		break;
	case SceneId::Tictactoe:
		mpCurrentScene = mpTictactoe;
		break;
	default:
		break;
	}
}

void SceneManager::input(const a3_DemoState* demoState)
{
	//Key was pressed
	//if (demoState->keyboard->key.key['b']) - Current key state
	//if (a3keyboardGetState(demoState->keyboard, a3key_B) > 0) - Current key state
	//if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0) - If key was just pressed or released

	mpCurrentScene->input(demoState);
}

void SceneManager::networkReceive(const a3_DemoState* demoState)
{
	mpCurrentScene->networkReceive(demoState);
}

void SceneManager::update(const a3_DemoState* demoState)
{
	mpCurrentScene->update(demoState);
}

void SceneManager::networkSend(const a3_DemoState* demoState)
{
	mpCurrentScene->networkSend(demoState);
}

void SceneManager::render(const a3_DemoState* demoState)
{
	glClear(GL_COLOR_BUFFER_BIT);
	mpCurrentScene->render(demoState);
}