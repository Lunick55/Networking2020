#ifndef SERVERBOIDS_SHUTDOWNCONFIRMATION_H_
#define SERVREBOIDS_SHUTDOWNCONFIRMATION_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class ServerBoidsShutdownConfirmation : public SceneState
{
public:
	ServerBoidsShutdownConfirmation(std::shared_ptr<Scene> parentScene);
	virtual ~ServerBoidsShutdownConfirmation() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;
};

#endif