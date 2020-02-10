#include "andrick_scene.h"
#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"

Scene::Scene(const SceneId id) :
	mId(id)
{

}

const SceneId Scene::getId() const 
{ 
	return mId; 
}

bool Scene::isKeyPressed(const a3_DemoState* demoState, a3_KeyboardKey key)
{
	return a3keyboardIsChanged(demoState->keyboard, key) > 0;
}