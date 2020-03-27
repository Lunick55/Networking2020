#ifndef SERVERBOIDS_CONTROLPANEL_H_
#define SERVREBOIDS_CONTROLPANEL_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class ServerBoidsControlPanel : public SceneState
{
public:
	ServerBoidsControlPanel(std::shared_ptr<Scene> parentScene);
	virtual ~ServerBoidsControlPanel() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

	void handleBoidDataEvents(std::shared_ptr<BoidDataEvent> boidEvnt);

private:
	unsigned int mChatHistory;
	std::vector<std::shared_ptr<struct LogInfo>> mChatLogHistory;
	std::string mDataModeText;
	PacketEventId mDataMode;
};

#endif