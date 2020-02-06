#ifndef ANDRICK_SCENE_MAINMENU_H_
#define ANDRICK_SCENE_MAINMENU_H_

#include "andrick_scene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() = default;

protected:
	virtual void input(const a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:

};

#endif