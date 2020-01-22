#ifndef SCENE_H_
#define SCENE_H_

#include "Common.h"
#include <conio.h>

class SceneManager;

class Scene
{
public:
	Scene(SceneId id);

	inline SceneId getId() const { return mId; }

	virtual void update() = 0;
	virtual void render() = 0;
	virtual void handleInput(const char& input) = 0;
	virtual void drawInitialScene() = 0;
	
	virtual void enteringScene();

	static void requestQuit();
	static void clearScreen();

	void clearScreenPortion(std::size_t startX, std::size_t startY, std::size_t lengthX, std::size_t lengthY);

	//Returns the console cursor x position.
	static int getConsoleCursorX();

	//Returns the console cursor y position.
	static int getConsoleCursorY();

	//Returns the console width.
	static int getConsoleWidth();

	//Returns the console height.
	static int getConsoleHeight();

	//Sets the cursor to the correct position to draw in the center of the screen depending on the width entered.
	static void setCursorForCenterText(int width);
	
	//Sets the cursor position in the console.
	static void setCursorPosition(std::size_t x, std::size_t y);

	//Sets the cursor to the correct position to draw the given string at the center of the width entered.
	std::string generateCenteredText(std::string text, std::size_t width = getConsoleWidth());

	void drawCenteredText(const std::string& text, std::size_t width = getConsoleWidth());

	//Draws a line across the screen at the given line height.
	void drawLine(int lineHeight);

protected:
	std::string mCurrentInput;

	//Might do something with the console later
	void clearInput();

private:
	SceneId mId;
};

class MenuScene : public Scene
{
public:
	inline MenuScene() : Scene(SceneId::MAIN_MENU) {}
	virtual void update() override;
	virtual void render() override;
	virtual void handleInput(const char& input) override;
	virtual void drawInitialScene() override;
};

class JoinChatRoomScene : public Scene
{
public:
	inline JoinChatRoomScene() : Scene(SceneId::JOIN_CHATROOM) {}
	virtual void update() override;
	virtual void render() override;
	virtual void handleInput(const char& input) override;
	virtual void drawInitialScene() override;

private:
	enum StepId
	{
		ENTER_IP = 0,
		ENTER_USERNAME
	};

	int mStepCount = ENTER_IP;
	std::string mIP;
	std::string mUsername;
	const int mSTEP1_CLEAR_START_HEIGHT = 6;
	const int mSTEP1_CLEAR_HEIGHT = 10;
	const int mSTEP2_CLEAR_START_HEIGHT = 14;
	const int mSTEP2_CLEAR_HEIGHT = 10;
};

class CreateChatRoomScene : public Scene
{
public:
	inline CreateChatRoomScene() : Scene(SceneId::CREATE_CHATROOM), mMaxUsers(sDEFAULT_MAX_USERS) {}
	virtual void update() override;
	virtual void render() override;
	virtual void handleInput(const char& input) override;
	virtual void drawInitialScene() override;

private:
	enum StepId
	{
		ENTER_MAX_USERS = 0,
		ENTER_USERNAME
	};

	int mStepCount = ENTER_MAX_USERS;
	int mMaxUsers;
	const int mSTEP1_CLEAR_START_HEIGHT = 7;
	const int mSTEP1_CLEAR_HEIGHT = 10;
	const int mSTEP2_CLEAR_START_HEIGHT = 14;
	const int mSTEP2_CLEAR_HEIGHT = 10;
	std::string mUsername;

	void initChatRoom();
};

class ChatRoomScene : public Scene
{
public:
	inline ChatRoomScene() : Scene(SceneId::CHATROOM) {}
	virtual void update() override;
	virtual void render() override;
	virtual void handleInput(const char& input) override;
	virtual void drawInitialScene() override;
};

#endif