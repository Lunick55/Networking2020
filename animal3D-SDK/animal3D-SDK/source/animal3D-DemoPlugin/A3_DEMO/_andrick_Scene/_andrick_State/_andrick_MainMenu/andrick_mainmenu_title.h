#ifndef MAINMENU_TITLE_H_
#define MAINMENU_TITLE_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class MainMenuTitle : public SceneState
{
public:
	MainMenuTitle(std::shared_ptr<class Scene> parentScene);
	virtual ~MainMenuTitle() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void update() override;
	virtual void render() override;
	virtual void exitingState() override;
};

#endif