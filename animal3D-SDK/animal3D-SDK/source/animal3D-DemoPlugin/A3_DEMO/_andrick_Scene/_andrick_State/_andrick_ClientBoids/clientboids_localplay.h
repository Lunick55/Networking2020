#ifndef CLIENTBOIDS_LOCALPLAY_H_
#define CLIENTBOIDS_LOCALPLAY_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>

class ClientBoidsLocalPlay : public SceneState
{
public:
	ClientBoidsLocalPlay(std::shared_ptr<Scene> parentScene);
	virtual ~ClientBoidsLocalPlay() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;
};

#endif