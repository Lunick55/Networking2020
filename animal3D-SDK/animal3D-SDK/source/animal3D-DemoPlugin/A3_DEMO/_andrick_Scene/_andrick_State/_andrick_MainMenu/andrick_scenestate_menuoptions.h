#ifndef SCENESTATE_MENUOPTIONS_H_
#define SCENESTATE_MENUOPTIONS_H_

#include <A3_DEMO/_andrick_Scene/_andrick_State/andrick_scenestate.h>

struct MenuOption
{
	MenuOption(const std::string& text, SceneId scene, Scene::SceneStateId state) :
		optionText(text), sceneId(scene), stateId(state) {}

	std::string optionText;
	SceneId sceneId;
	Scene::SceneStateId stateId;
};

class SceneStateMenuOptions : public SceneState
{
public:
	SceneStateMenuOptions(std::weak_ptr<class Scene> parentScene);
	virtual ~SceneStateMenuOptions() = default;

	virtual void enteringState() override;
	virtual void processInput() override;
	virtual void update() override;
	virtual void render() override;
	virtual void exitingState() override;

private:
	std::vector<MenuOption> mMenuOptions;
};

#endif