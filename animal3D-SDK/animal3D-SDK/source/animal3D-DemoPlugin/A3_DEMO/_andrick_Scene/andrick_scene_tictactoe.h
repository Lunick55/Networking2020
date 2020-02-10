#ifndef ANDRICK_SCENE_TICTACTOE_H_
#define ANDRICK_SCENE_TICTACTOE_H_

#include "andrick_scene.h"

class TictactoeScene : public Scene
{
public:
	TictactoeScene();
	virtual ~TictactoeScene() = default;

protected:
	virtual void enteringScene(const a3_DemoState* demoState) override;
	virtual void input(a3_DemoState* demoState) override;
	virtual void networkReceive(const a3_DemoState* demoState) override;
	virtual void update(const a3_DemoState* demoState) override;
	virtual void networkSend(const a3_DemoState* demoState) override;
	virtual void render(const a3_DemoState* demoState) override;

private:

};

#endif