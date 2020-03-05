#ifndef CLIENTBOIDS_CLIENTWORLD_H_
#define CLIENTBOIDS_CLIENTWORLD_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class ClientBoidsClientWorld : public SceneState
{
public:
	ClientBoidsClientWorld(std::shared_ptr<Scene> parentScene);
	virtual ~ClientBoidsClientWorld() = default;

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
	std::map<PacketEventId, std::string> mDataModeMap;
};

#endif