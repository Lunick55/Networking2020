#ifndef MINIGAME_MAIN_H_
#define MINIGAME_MAIN_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class MiniGameMain : public SceneState
{
public:
	MiniGameMain(class Scene& parentScene);
	virtual ~MiniGameMain() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	//any variables needed
	int numberOfTimesSpacePressed = 0;

	void IncrementSpace(); //a silly way to test the event system
};

#endif