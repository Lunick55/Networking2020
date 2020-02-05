#ifndef ANDRICK_SCENE_MAINMENU_H_
#define ANDRICK_SCENE_MAINMENU_H_

#include "andrick_scene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() = default;

protected:
	virtual void update() override;
	virtual void render() override;

private:

};

#endif