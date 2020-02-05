#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <string>

enum class SceneId
{
	MainMenu = 0,
	Count
};

class Scene
{
public:
	explicit Scene(const SceneId id);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	const SceneId getId() const;

protected:
	virtual void update() = 0;
	virtual void render() = 0;

private:
	const SceneId mId;
};

#endif