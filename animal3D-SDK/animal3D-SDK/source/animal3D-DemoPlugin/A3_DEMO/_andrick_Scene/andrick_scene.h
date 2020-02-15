#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <GL/glew.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class Scene
{
	friend class SceneManager;

public:
	explicit Scene(const SceneId id, const Color backgroundColor = AndrickColors::DARK_GREY);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	const SceneId getId() const;

protected:
	std::shared_ptr<class SceneInputHandler> mpInputHandler;

	virtual void enteringScene();
	virtual void input();
	virtual void processIncomingEvents();
	virtual void update() = 0;
	virtual void packageOutgoingEvents();
	virtual void render();

	//Have a message convenience class
	void sendPublicMessage(const std::string& message, MessageType type);

private:
	const SceneId mId;
	Color mBackgroundColor;
};

#endif