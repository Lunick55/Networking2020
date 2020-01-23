#ifndef CHAT_ROOM_SCENE_H_
#define CHAT_ROOM_SCENE_H_

#include "Scene.h"
#include "ChatRoomServer.h"

class ChatRoomScene : public Scene
{
public:
	inline ChatRoomScene() : Scene(SceneId::CHATROOM) {}
	virtual void update() override;
	virtual void render() override;
	virtual void handleInput(const char& input) override;
	virtual void drawInitialScene() override;
	virtual void enteringScene() override;
};

#endif