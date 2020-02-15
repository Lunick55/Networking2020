#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>

SceneState::SceneState(std::weak_ptr<Scene> parentScene, Scene::SceneStateId id, Color backgroundColor) :
	mpParentScene(parentScene),
	mID(id),
	mpInputHandler(std::make_shared<SceneInputHandler>()),
	mBackgroundColor(backgroundColor) {}

void SceneState::enteringState()
{
	mpInputHandler->clear();
}

void SceneState::processInput()
{
	mpInputHandler->getInput();
}

void SceneState::render()
{
	glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
}

void SceneState::exitingState()
{
	mpInputHandler->clear();
}

const Scene::SceneStateId SceneState::getId() const
{ 
	return mID; 
}