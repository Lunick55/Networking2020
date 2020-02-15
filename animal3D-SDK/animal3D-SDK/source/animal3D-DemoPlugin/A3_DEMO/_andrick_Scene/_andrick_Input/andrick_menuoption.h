#ifndef MENU_OPTION_H_
#define MENU_OPTION_H_

#include <A3_DEMO/_andrick_Scene/andrick_scene.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>

struct MenuOption
{
	MenuOption(a3_KeyboardKey selector, const std::string& text, SceneId scene, SceneStateId state) :
		selectKey(selector),
		optionText(text), 
		sceneId(scene), 
		stateId(state) 
	{}

	a3_KeyboardKey selectKey;
	std::string optionText;
	SceneId sceneId;
	SceneStateId stateId;
};

#endif