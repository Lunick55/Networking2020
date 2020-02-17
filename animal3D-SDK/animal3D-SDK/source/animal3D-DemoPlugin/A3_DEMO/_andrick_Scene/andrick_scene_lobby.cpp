#include <A3_DEMO/_andrick_Scene/andrick_scene_lobby.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_Lobby/andrick_lobby_chatroom.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

LobbyScene::LobbyScene() :
	Scene(SceneId::LOBBY)
{
	//init scene states
	setDefaultState(std::make_shared<LobbyChatroom>(*this));
}
