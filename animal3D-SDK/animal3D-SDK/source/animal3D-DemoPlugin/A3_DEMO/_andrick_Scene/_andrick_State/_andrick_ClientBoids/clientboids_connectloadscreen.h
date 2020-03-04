#ifndef CLIENTBOIDS_CONNECTLOADSCREEN_H_
#define CLIENTBOIDS_CONNECTLOADSCREEN_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

class ClientBoidsConnectLoadScreen : public SceneState
{
public:
	ClientBoidsConnectLoadScreen(std::shared_ptr<Scene> parentScene);
	virtual ~ClientBoidsConnectLoadScreen() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void processIncomingEvent(std::shared_ptr<Event> evnt) override;
	virtual void update() override;
	virtual void queueOutgoingEvents() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	Color mNormalTextColor;
	Color mErrorTextColor;
	Color mOutputTextColor;
	
	std::string mLoadText;
	std::string mDotText;
	std::string mConnectionTimedOut;

	std::string mOutputText;

	const a3f64 mCONNECTION_TIMEOUT_TIME = 5.0f;
	a3f64 mTimeoutUpdateCounter = 0;

	const a3f64 mCHANGE_DOT_AFTER_SECONDS = 1.0f;
	const std::size_t mMAX_DOT_LENGTH = 3;
	a3f64 mDotUpdateCounter = 0;
	bool mShouldUpdate = true;
};

#endif