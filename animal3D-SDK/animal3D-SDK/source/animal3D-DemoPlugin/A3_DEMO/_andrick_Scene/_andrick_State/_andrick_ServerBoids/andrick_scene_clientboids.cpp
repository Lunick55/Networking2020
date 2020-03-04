#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_joinip.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_joinusername.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_clientworld.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_confirmdisconnect.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_localplay.h>
#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_connectloadscreen.h>

ClientBoidsScene::ClientBoidsScene() :
	Scene(SceneId::CLIENT_BOIDS)
{
	//init scene states
	std::shared_ptr<ClientBoidsScene> instance = std::shared_ptr<ClientBoidsScene>(this);
	setDefaultState(std::make_shared<ClientBoidsJoinIP>(instance));
	initSceneState(std::make_shared<ClientBoidsJoinUsername>(instance));
	initSceneState(std::make_shared<ClientBoidsClientWorld>(instance));
	initSceneState(std::make_shared<ClientBoidsConfirmDisconnect>(instance));
	initSceneState(std::make_shared<ClientBoidsLocalPlay>(instance));
	initSceneState(std::make_shared<ClientBoidsConnectLoadScreen>(instance));
}