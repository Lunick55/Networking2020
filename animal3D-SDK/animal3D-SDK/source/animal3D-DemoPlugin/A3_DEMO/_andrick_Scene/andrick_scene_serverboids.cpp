#include <A3_DEMO/_andrick_Scene/andrick_scene_serverboids.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ServerBoids/andrick_serverboids_ctlpanel.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ServerBoids/andrick_serverboids_shutdown.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>

ServerBoidsScene::ServerBoidsScene() :
	Scene(SceneId::SERVER_BOIDS)
{
	//init scene states
	std::shared_ptr<ServerBoidsScene> instance = std::shared_ptr<ServerBoidsScene>(this);
	setDefaultState(std::make_shared<ServerBoidsControlPanel>(instance));
	initSceneState(std::make_shared<ServerBoidsShutdownConfirmation>(instance));
}
