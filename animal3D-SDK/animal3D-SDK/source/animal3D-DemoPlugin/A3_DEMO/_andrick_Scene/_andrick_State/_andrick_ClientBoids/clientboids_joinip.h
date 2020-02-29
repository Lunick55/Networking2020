#ifndef CLIENTBOIDS_JOINIP_H_
#define CLIENTBOIDS_JOINIP_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>
#include <string>

class ClientBoidsJoinIP : public SceneState
{
public:
	ClientBoidsJoinIP(std::shared_ptr<Scene> parentScene);
	virtual ~ClientBoidsJoinIP() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	std::string mNormalText;
	std::string mInvalidIPText;
	std::string mFailedToConnectText;
	std::string mOutputText;
	Color mNormalTextColor;
	Color mErrorTextColor;
	Color mOutputTextColor;
};

#endif