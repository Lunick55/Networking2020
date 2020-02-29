#ifndef ANDRICK_SCENE_CLIENTBOIDS_H_
#define ANDRICK_SCENE_CLIENTBOIDS_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class ClientBoidsScene : public Scene
{
public:
	const enum class ClientBoidsStateId : char
	{
		INVALID_STATE = (char)SceneStateId::INVALID_STATE,
		JOIN_IP,
		JOIN_USERNAME,
		CLIENT_WORLD,
		CONFIRM_DISCONNECT,
		LOCAL_PLAY
	};

	ClientBoidsScene();
	virtual ~ClientBoidsScene() = default;

protected:
	//virtual void input() override;
	//virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	//virtual void update() override;
	//virtual void queueOutgoingEvents() override;
	//virtual void render() override;

private:

};

#endif