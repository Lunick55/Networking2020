#include "andrick_scene.h"

Scene::Scene(const SceneId id) :
	mId(id)
{

}

const SceneId Scene::getId() const 
{ 
	return mId; 
}