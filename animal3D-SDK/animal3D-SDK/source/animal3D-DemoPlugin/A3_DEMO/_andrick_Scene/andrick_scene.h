#ifndef ANDRICK_SCENE_H_
#define ANDRICK_SCENE_H_

#include <string>
#include <vector>
#include <GL/glew.h>

#include "A3_DEMO/_andrick_Utils/_andrick_text_formatter.h"
#include "A3_DEMO/_andrick_Utils/andrick_common.h"

struct a3_DemoState;
enum a3_KeyboardKey;

struct LogInfo
{
	MessageType type;
	std::string text;
	a3vec4 color;
};

class Scene
{
	friend class SceneManager;
public:
	explicit Scene(const SceneId id);
	virtual ~Scene() = default;

	Scene(const Scene& scene) = delete;
	Scene& operator=(const Scene& scene) = delete;

	const SceneId getId() const;

	void addToChatList(MessageType type, const std::string& msg, unsigned int newLineAmount = 1, a3vec4 color = TextFormatter::WHITE);
	void addToChatList(LogInfo logInfo, unsigned int newLineAmount = 1);

protected:
	std::string mCurrentInput;
	std::vector<LogInfo> mChatLog;

	virtual void enteringScene(const a3_DemoState* demoState);
	virtual void input(a3_DemoState* demoState);
	virtual void networkReceive(const a3_DemoState* demoState);
	virtual void update(const a3_DemoState* demoState) = 0;
	virtual void networkSend(const a3_DemoState* demoState);
	virtual void render(const a3_DemoState* demoState) = 0;

	//Key was pressed - animal references
	//if (demoState->keyboard->key.key['b']) - Current key state
	//if (a3keyboardGetState(demoState->keyboard, a3key_B) > 0) - Current key state
	//if (a3keyboardIsChanged(demoState->keyboard, a3key_enter) > 0) - If key was just pressed or released
	bool isKeyPressed(const a3_DemoState* demoState, a3_KeyboardKey key);
	bool isKeyHeld(const a3_DemoState* demoState, a3_KeyboardKey key);
	bool isKeyReleased(const a3_DemoState* demoState, a3_KeyboardKey key);

	bool handleBackspace(const a3_DemoState* demoState, std::string& input);
	void handleTyping(const a3_DemoState* demoState, std::string& input);

	bool validateUsername(std::string& input);
	bool validateNumber(const std::string& input, int& number);

	bool isCommand(const std::string& input);
	bool processCommand(const a3_DemoState* demoState, const std::string& input, std::string& commandOutput, std::string& outputResults);
	bool processWhisperCommand(const a3_DemoState* demoState, const std::string& commandArgs);
	bool processStartGameCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults);
	bool processListCommand(const a3_DemoState* demoState);
	bool processSelectPlayersCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults);
	bool processPlayTictacCommand(const a3_DemoState* demoState, const std::string& commandArgs, std::string& outputResults);

	void sendPublicMessage(const a3_DemoState* demoState, const std::string& message, MessageType type);

private:
	const SceneId mId;
};

#endif