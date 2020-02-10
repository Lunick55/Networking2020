#ifndef ANDRICK_SCENE_SELECTROLE_H_
#define ANDRICK_SCENE_SELECTROLE_H_

#include "andrick_scene.h"

class SelectRoleScene : public Scene
{
public:
	SelectRoleScene();
	virtual ~SelectRoleScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:
	enum class SelectRoleStep : a3byte
	{
		CLOSE_PROGRAM = -1,
		IS_CLIENT_OR_HOST,

		CLIENT_ENTER_USERNAME,
		CLIENT_ENTER_IP,
		CLIENT_VALIDATE_IP,

		HOST_ENTER_USERNAME,
		HOST_MAX_CLIENTS,
		HOST_VALIDATE_MAX_CLIENTS
	};

	SelectRoleStep mCurrentStep;
	std::string mUsername;
	std::string mIP;
	bool mIsHost;
};

#endif