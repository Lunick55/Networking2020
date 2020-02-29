#ifndef MENU_OPTION_H_
#define MENU_OPTION_H_

#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <functional>
#include <optional>

struct MenuOption
{
	MenuOption() {}

	MenuOption(a3_KeyboardKey selector, const std::string& text, std::function<void()> chosen = nullptr, std::optional<SceneId> scene = std::nullopt, std::optional<SceneStateId> state = std::nullopt) :
		selectKey(selector),
		optionText(text), 
		sceneId(scene), 
		stateId(state),
		onOptionChosen(chosen)
	{}

	virtual void execute()
	{
		if (onOptionChosen)
		{
			onOptionChosen();
		}
	}

	bool operator==(const MenuOption& other)
	{
		return (
			selectKey == other.selectKey
		&&	optionText.compare(other.optionText) == 0
		&&	sceneId == other.sceneId
		&&	stateId == other.stateId
		);
	}

	a3_KeyboardKey selectKey;
	std::string optionText;
	std::optional<SceneId> sceneId;
	std::optional<SceneStateId> stateId;
	std::function<void()> onOptionChosen;
};

#endif