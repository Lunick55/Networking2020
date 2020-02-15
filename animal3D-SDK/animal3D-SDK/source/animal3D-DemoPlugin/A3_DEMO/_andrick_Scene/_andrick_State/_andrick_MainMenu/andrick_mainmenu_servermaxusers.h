#ifndef MAINMENU_SERVERMAXUSERS_H_
#define MAINMENU_SERVERMAXUSERS_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class MainMenuServerMaxUsers : public SceneState
{
public:
	MainMenuServerMaxUsers(std::shared_ptr<class Scene> parentScene);
	virtual ~MainMenuServerMaxUsers() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void update() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	std::string mNormalText;
	std::string mErrorText;
	std::string mOutputText;
	Color mNormalTextColor;
	Color mErrorTextColor;
	Color mOutputTextColor;
};

#endif