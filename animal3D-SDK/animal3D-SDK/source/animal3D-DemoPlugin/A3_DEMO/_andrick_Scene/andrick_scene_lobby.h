#ifndef ANDRICK_SCENE_LOBBY_H_
#define ANDRICK_SCENE_LOBBY_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene() = default;

protected:
	virtual void enteringScene() override;
	virtual void input() override;
	virtual void processIncomingEvents() override;
	virtual void update() override;
	virtual void packageOutgoingEvents() override;
	virtual void render() override;

private:
	//Create a step class and map to the steps
	enum class LobbyStep : a3byte
	{
		LEAVE_SERVER = -1,
		LEAVE_SERVER_CONFIRM,
		CHATROOM
	};

	LobbyStep mCurrentStep;
	GameType mSelectedGame;
	bool mInitSuccessful;

	void handleInputChatRoom();
	void handleInputLeaveServerConfirm();

	void handleUpdateLeaveServer();

	void handleNetworkSendChatRoomStep();
};

#endif