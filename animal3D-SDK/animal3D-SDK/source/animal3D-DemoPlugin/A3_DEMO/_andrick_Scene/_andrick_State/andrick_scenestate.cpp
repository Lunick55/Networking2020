#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>

SceneState::SceneState(std::shared_ptr<Scene> parentScene, SceneStateId id, Color backgroundColor) :
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
	for (auto iter = mMenuOptions.begin(); iter != mMenuOptions.end(); ++iter)
	{
		if (mpInputHandler->isKeyPressed(iter->selectKey))
		{
			if (iter->sceneId == SceneId::EXIT)
			{
				gDemoState->exitFlag = -1;
				return;
			}
			else if (iter->sceneId == mpParentScene->getId())
			{
				mpParentScene->switchToState(iter->stateId);
			}
			else
			{
				gDemoState->mpSceneManager->switchToScene(iter->sceneId, mID);
			}
		}
	}

	mpInputHandler->getInput();
}

void SceneState::render()
{
	glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
}

void SceneState::renderMenuOptions(Color color, TextAlign alignment, a3vec3 viewportPosition)
{
	for (auto iter = mMenuOptions.begin(); iter != mMenuOptions.end(); ++iter)
	{
		gTextFormatter.drawText(iter->optionText, color, alignment, viewportPosition);
		gTextFormatter.offsetLine(2);
	}
}

void SceneState::exitingState()
{
	mpInputHandler->clear();
}

const SceneStateId SceneState::getId() const
{ 
	return mID; 
}