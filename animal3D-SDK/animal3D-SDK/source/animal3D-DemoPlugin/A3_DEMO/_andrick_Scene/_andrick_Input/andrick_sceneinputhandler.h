#ifndef ANDRICK_SCENEINPUTHANDLER_H_
#define ANDRICK_SCENEINPUTHANDLER_H_

#include <memory>
#include <string>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>

class SceneInputHandler
{
public:
	explicit SceneInputHandler();
	virtual ~SceneInputHandler() = default;

	SceneInputHandler(const SceneInputHandler& scene) = delete;
	SceneInputHandler& operator=(const SceneInputHandler& scene) = delete;

	void clear();
	void clearCurrentInput();

	void updateInput();
	const std::string& getCurrentInput() const;

	bool handleCommandInput();
	bool isCommand();
	bool processCommand(std::shared_ptr<struct Command>& out);

	bool handleCommandInput(const std::string& input);
	bool isCommand(const std::string& input);
	bool processCommand(const std::string& input, std::shared_ptr<struct Command>& out);

	bool isKeyPressed(enum a3_KeyboardKey key);
	bool isKeyHeld(enum a3_KeyboardKey key);
	bool isKeyReleased(enum a3_KeyboardKey key);

	bool handleBackspace(std::string& input);
	void handleTyping(std::string& input);

	std::shared_ptr<class ChatLog> getChatLog();

	bool validateUsername(std::string& input);
	bool validateNumber(const std::string& input, int& number);

private:
	std::shared_ptr<class ChatLog> mpChatLog;
	std::string mCurrentInput;
};

#endif