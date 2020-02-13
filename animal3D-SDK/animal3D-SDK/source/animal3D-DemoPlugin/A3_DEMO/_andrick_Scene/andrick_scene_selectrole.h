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
		CLIENT_CONNECT_TO_SERVER,

		HOST_ENTER_USERNAME,
		HOST_MAX_CLIENTS,
		HOST_INVALID_MAX_CLIENTS,

		INVALID_USERNAME
	};

	SelectRoleStep mCurrentStep;
	std::string mUsername;
	std::string mIP;
	bool mIsHost;

	bool handleInputEscape(a3_DemoState* demoState, SelectRoleStep targetStep);

	void handleInputIsHostOrClient(a3_DemoState* demoState);
	void handleInputClientEnterUsername(a3_DemoState* demoState);
	void handleInputClientEnterIP(a3_DemoState* demoState);
	void handleInputClientConnectToServer(a3_DemoState* demoState);
	void handleInputHostEnterUsername(a3_DemoState* demoState);
	void handleInputHostMaxClients(a3_DemoState* demoState);
	void handleInputHostInvalidMaxClients(a3_DemoState* demoState);
	void handleInputInvalidUsername(a3_DemoState* demoState);

	void handleInputUsername(a3_DemoState* demoState);
	//void handleInvalidInput(a3_DemoState* demoState);


	void renderIsHostOrClient(			const a3_DemoState* demoState, TextFormatter& formatter);
	void renderClientEnterUsername(		const a3_DemoState* demoState, TextFormatter& formatter);
	void renderClientEnterIP(			const a3_DemoState* demoState, TextFormatter& formatter);
	void renderClientConnectToServer(	const a3_DemoState* demoState, TextFormatter& formatter);
	void renderHostEnterUsername(		const a3_DemoState* demoState, TextFormatter& formatter);
	void renderHostMaxClients(			const a3_DemoState* demoState, TextFormatter& formatter);
	void renderHostInvalidMaxClients(	const a3_DemoState* demoState, TextFormatter& formatter);
	void renderInvalidUsername(			const a3_DemoState* demoState, TextFormatter& formatter);
	void renderUsername(				const a3_DemoState* demoState, TextFormatter& formatter);

};

#endif