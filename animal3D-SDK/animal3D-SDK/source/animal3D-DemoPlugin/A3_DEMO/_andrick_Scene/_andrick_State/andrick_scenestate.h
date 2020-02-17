#ifndef SCENE_STATE_H_
#define SCENE_STATE_H_

#include <GL/glew.h>
#include <set>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Event/andrick_event.h>

class SceneState
{
	friend class Scene;

public:
	SceneState(Scene& parentScene, SceneStateId id, Color backgroundColor = DARK_GREY);
	virtual ~SceneState() = default;

	SceneState(const SceneState& scene) = delete;
	SceneState& operator=(const SceneState& scene) = delete;

	virtual void enteringState();
	virtual void processInput();
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt);
	virtual void update() = 0;
	virtual void queueOutgoingEvents() = 0;
	virtual void render();
	virtual void exitingState();

	void addValidCommand(CommandId commandId);
	void removeValidCommand(CommandId commandId);
	virtual void handleCommandEvent(std::shared_ptr<CommandEvent> commandEvnt);

	const enum SceneStateId getId() const;

	void renderMenuOptions(Color color, TextAlign alignment, unsigned int spacing = 2, a3vec3 viewportPosition = a3vec3());
	void renderChatLogHistory(const std::vector<std::shared_ptr<struct LogInfo>>& chatLogHistory, TextAlign alignment, unsigned int spacing = 2, a3vec3 viewportPosition = a3vec3());

protected:
	Scene& mParentScene;
	const SceneStateId mID;
	std::shared_ptr<class SceneInputHandler> mpInputHandler;
	std::vector<struct MenuOption> mMenuOptions;
	Color mBackgroundColor;

private:
	std::set<CommandId> mValidCommands;
};

#endif