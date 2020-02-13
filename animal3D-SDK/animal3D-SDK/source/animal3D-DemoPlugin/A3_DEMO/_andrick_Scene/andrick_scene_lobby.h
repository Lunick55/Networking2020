#ifndef ANDRICK_SCENE_LOBBY_H_
#define ANDRICK_SCENE_LOBBY_H_

#include "andrick_scene.h"

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:
	enum class LobbyStep : a3byte
	{
		LEAVE_SERVER = -1,
		LEAVE_SERVER_CONFIRM,
		CHATROOM
	};

	LobbyStep mCurrentStep;
	GameType mSelectedGame;
	bool mInitSuccessful;

	void handleInputChatRoom(a3_DemoState* demoState);
	void handleInputLeaveServerConfirm(const a3_DemoState* demoState);

	void handleUpdateLeaveServer(const a3_DemoState* demoState);

	void handleNetworkSendChatRoomStep(const a3_DemoState* demoState);
};

#endif