#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <string>
#include <GL/glew.h>

#include "A3_DEMO/_andrick_Utils/_andrick_text_formatter.h"
#include "A3_DEMO/_andrick_Utils/andrick_common.h"

struct a3_DemoState;
enum a3_KeyboardKey;

class Scene
{
	friend class SceneManager;

public:
	explicit Scene(const SceneId id);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	const SceneId getId() const;

protected:
	std::string mCurrentInput;

	virtual void input(a3_DemoState* demoState) = 0;
	virtual void networkReceive(const a3_DemoState* demoState) = 0;
	virtual void update(const a3_DemoState* demoState) = 0;
	virtual void networkSend(const a3_DemoState* demoState) = 0;
	virtual void render(const a3_DemoState* demoState) = 0;

	//Key was pressed - animal references
	//if (demoState->keyboard->key.key['b']) - Current key state
	//if (a3keyboardGetState(demoState->keyboard, a3key_B) > 0) - Current key state
	//if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0) - If key was just pressed or released
	bool isKeyPressed(const a3_DemoState* demoState, a3_KeyboardKey key);

private:
	const SceneId mId;
};

#endif