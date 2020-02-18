#include <A3_DEMO/_andrick_Scene/andrick_scene_miniGame.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_MiniGame/andrick_miniGame_main.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

MiniGameScene::MiniGameScene() : Scene(SceneId::MINIGAME)
{
	setDefaultState(std::make_shared<MiniGameMain>(*this));
}