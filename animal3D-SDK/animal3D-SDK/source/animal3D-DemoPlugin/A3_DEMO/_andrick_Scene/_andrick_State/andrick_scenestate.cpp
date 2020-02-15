#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

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

			mpParentScene->switchToState(iter->sceneId, iter->stateId);
		}
	}

	mpInputHandler->updateInput();
}

void SceneState::render()
{
	glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
}

void SceneState::renderMenuOptions(Color color, TextAlign alignment, unsigned int spacing, a3vec3 viewportPosition)
{
	for (auto iter = mMenuOptions.begin(); iter != mMenuOptions.end(); ++iter)
	{
		gTextFormatter.drawText(iter->optionText, color, alignment, viewportPosition);
		gTextFormatter.offsetLine(spacing);
	}
}

void SceneState::renderChatLogHistory(const std::vector<std::shared_ptr<struct LogInfo>>& chatLogHistory, TextAlign alignment, unsigned int spacing, a3vec3 viewportPosition)
{
	for (auto iter = chatLogHistory.begin(); iter != chatLogHistory.end(); ++iter)
	{
		gTextFormatter.drawText((*iter)->text, (*iter)->color, alignment, viewportPosition);
		gTextFormatter.offsetLine(spacing);
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