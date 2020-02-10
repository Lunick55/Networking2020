#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_manager.h"
#include "andrick_scene_selectrole.h"
#include "andrick_scene_lobby.h"
#include "andrick_scene_tictactoe.h"

SceneManager::SceneManager() :
	mpSelectRole(new SelectRoleScene()),
	mpLobby(new LobbyScene()),
	mpTictactoe(new TictactoeScene())
{
	mpCurrentScene = mpSelectRole;
}

SceneManager::~SceneManager()
{
	delete mpSelectRole;
	mpSelectRole = nullptr;

	delete mpLobby;
	mpLobby = nullptr;

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
	case SceneId::Lobby:
		mpCurrentScene = mpLobby;
	default:
		break;
	}
}

void SceneManager::input(a3_DemoState* demoState)
{
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