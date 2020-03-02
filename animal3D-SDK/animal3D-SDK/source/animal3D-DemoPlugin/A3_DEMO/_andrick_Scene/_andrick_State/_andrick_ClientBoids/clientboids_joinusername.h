#ifndef CLIENTBOIDS_JOINUSERNAME_H_
#define CLIENTBOIDS_JOINUSERNAME_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class ClientBoidsJoinUsername : public SceneState
{
public:
	ClientBoidsJoinUsername(std::shared_ptr<Scene> parentScene);
	virtual ~ClientBoidsJoinUsername() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	std::string mNormalText;
	std::string mInvalidUsernameText;
	std::string mUsernameTakenText;
	std::string mOutputText;
	Color mNormalTextColor;
	Color mErrorTextColor;
	Color mOutputTextColor;
};

#endif