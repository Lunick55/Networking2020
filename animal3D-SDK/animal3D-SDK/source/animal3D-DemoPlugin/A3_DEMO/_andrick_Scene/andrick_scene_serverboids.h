#ifndef ANDRICK_SCENE_SERVERBOIDS_H_
#define ANDRICK_SCENE_SERVERBOIDS_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class ServerBoidsScene : public Scene
{
public:
	const enum class ServerBoidsStateId : char
	{
		INVALID_STATE = (char)SceneStateId::INVALID_STATE,
		CONTROL_PANEL,
		SHUTDOWN_SERVER_CONFIRMATION
	};

	ServerBoidsScene();
	virtual ~ServerBoidsScene() = default;

protected:
	//virtual void input() override;
	//virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	//virtual void update() override;
	//virtual void queueOutgoingEvents() override;
	//virtual void render() override;

private:

};

#endif