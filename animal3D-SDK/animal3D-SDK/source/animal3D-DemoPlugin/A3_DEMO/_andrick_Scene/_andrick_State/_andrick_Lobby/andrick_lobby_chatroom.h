#ifndef LOBBY_CHATROOM_H_
#define LOBBY_CHATROOM_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class LobbyChatroom : public SceneState
{
public:
	LobbyChatroom(std::shared_ptr<class Scene> parentScene);
	virtual ~LobbyChatroom() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvents() override;
	virtual void update() override;
	virtual void packageOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	unsigned int mChatHistory;
	std::vector<std::shared_ptr<struct LogInfo>> mChatLogHistory;
};

#endif