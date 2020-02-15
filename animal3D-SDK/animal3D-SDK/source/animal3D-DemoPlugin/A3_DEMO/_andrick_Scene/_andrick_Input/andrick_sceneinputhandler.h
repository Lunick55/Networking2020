#ifndef ANDRICK_SCENEINPUTHANDLER_H_
#define ANDRICK_SCENEINPUTHANDLER_H_

#include <memory>
#include <string>

class SceneInputHandler
{
public:
	explicit SceneInputHandler(const unsigned int linesToDisplay = 10);
	virtual ~SceneInputHandler() = default;

	SceneInputHandler(const SceneInputHandler& scene) = delete;
	SceneInputHandler& operator=(const SceneInputHandler& scene) = delete;

	void clear();

	void getInput();

	bool isCommand(const std::string& input);
	bool processCommand(const std::string& input, std::shared_ptr<struct Command>& out);

	bool isKeyPressed(enum a3_KeyboardKey key);
	bool isKeyHeld(enum a3_KeyboardKey key);
	bool isKeyReleased(enum a3_KeyboardKey key);

	bool handleBackspace(std::string& input);
	void handleTyping(std::string& input);

	bool validateUsername(std::string& input);
	bool validateNumber(const std::string& input, int& number);

private:
	std::unique_ptr<class ChatLog> mpChatLog;
	std::string mCurrentInput;
};

#endif