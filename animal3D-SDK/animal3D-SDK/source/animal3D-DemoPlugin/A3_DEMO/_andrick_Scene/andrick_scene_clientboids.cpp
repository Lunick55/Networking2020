#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

ClientBoidsScene::ClientBoidsScene() :
	Scene(SceneId::CLIENT_BOIDS)
{
	//init scene states
	std::shared_ptr<ClientBoidsScene> instance = std::shared_ptr<ClientBoidsScene>(this);
	setDefaultState(std::make_shared<ServerBoidsControlPanel>(instance));
	initSceneState(std::make_shared<ServerBoidsShutdownConfirmation>(instance));
}