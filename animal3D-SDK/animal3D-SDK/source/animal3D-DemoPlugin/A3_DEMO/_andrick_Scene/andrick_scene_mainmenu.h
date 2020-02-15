#ifndef ANDRICK_SCENE_MAINMENU_H_
#define ANDRICK_SCENE_MAINMENU_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>

class MainMenuScene : public Scene
{
public:
	const enum class MenuSceneStateId : char
	{
		INVALID_STATE = (char)SceneStateId::INVALID_STATE,
		TITLE_MENU,
		SERVER_MAX_USERS
	};

	MainMenuScene();
	virtual ~MainMenuScene() = default;

protected:
	//virtual void input() override;
	//virtual void processIncomingEvents() override;
	//virtual void update() override;
	//virtual void packageOutgoingEvents() override;
	//virtual void render() override;

private:

};

#endif