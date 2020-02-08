#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <string>
#include <GL/glew.h>

#include "A3_DEMO/_andrick_Utils/_andrick_text_formatter.h"
#include "A3_DEMO/_andrick_Utils/andrick_common.h"

struct a3_DemoState;

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
	virtual void input(const a3_DemoState* demoState) = 0;
	virtual void networkReceive(const a3_DemoState* demoState) = 0;
	virtual void update(const a3_DemoState* demoState) = 0;
	virtual void networkSend(const a3_DemoState* demoState) = 0;
	virtual void render(const a3_DemoState* demoState) = 0;

private:
	const SceneId mId;
};

#endif