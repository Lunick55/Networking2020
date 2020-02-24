#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_manager.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>
#include <A3_DEMO/_andrick_Command/andrick_command.h>

SceneState::SceneState(Scene& parentScene, SceneStateId id, Color backgroundColor) :
	mParentScene(parentScene),
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

			iter->execute();
			mParentScene.switchToState(iter->sceneId, iter->stateId);
		}
	}

	mpInputHandler->updateInput();
}

void SceneState::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::COMMAND:
		handleCommandEvent(std::dynamic_pointer_cast<CommandEvent>(evnt));
		break;
	default:
		break;
	}
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

void SceneState::addValidCommand(CommandId commandId)
{
	mValidCommands.insert(commandId);
}

void SceneState::removeValidCommand(CommandId commandId)
{
	mValidCommands.erase(commandId);
}

void SceneState::handleCommandEvent(std::shared_ptr<CommandEvent> commandEvnt)
{
	std::set<CommandId>::iterator iter = mValidCommands.find(commandEvnt->command->ID);

	if (iter != mValidCommands.end())
	{
		commandEvnt->execute();
	}
}