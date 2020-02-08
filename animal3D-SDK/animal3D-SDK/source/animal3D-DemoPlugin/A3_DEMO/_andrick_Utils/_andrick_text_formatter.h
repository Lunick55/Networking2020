#ifndef ANDRICK_TEXT_FORMATTER_H_
#define ANDRICK_TEXT_FORMATTER_H_

#include <string>
#include "animal3D-A3DM/animal3D-A3DM.h"

struct a3_DemoState;

class TextFormatter
{
public:
	enum class TextAlign : a3byte
	{
		NONE,
		UNALIGN,
		LEFT,
		CENTER_X,
		CENTER_Y,
		CENTER,
		RIGHT
	};

	//Helper vars that will be used a lot
	static const float SCREEN_LEFT;
	static const float SCREEN_RIGHT;
	static const float SCREEN_TOP;
	static const float SCREEN_BOTTOM;
	static const float SCREEN_CENTER;
	static const float LINE_HEIGHT;
	static const float LETTER_SIZE;
	static a3vec4 WHITE;

	static TextFormatter& get()
	{
		static TextFormatter instance;
		return instance;
	}

	TextFormatter();
	void drawText(const a3_DemoState* demoState, const std::string& string, TextAlign alignOverride = TextAlign::NONE);
	//viewportPosition only affects the draw location if alignOverride is set to UNALIGN.
	void drawText(const a3_DemoState* demoState, const std::string& string, a3vec4 color, TextAlign alignOverride = TextAlign::NONE, a3vec3 viewportPosition = a3vec3());
	void newLine();
	void setLine(int lineNumber);
	void offsetLine(int offset);
	void setCursorPos(float x, float y, float z = -1.0f);
	void setColor(a3vec4 color);
	void setAlignment(TextAlign align);

	//Because a3vec4s structs don't have constructors to initalize the vars...
	static a3vec4 createColor(float x, float y, float z, float a);

private:
	a3vec3 mCursorPosition;
	a3vec4 mColor;
	TextAlign mAlignment;
	int mCurrentLine;

	a3vec3 calculatePosition(const std::string string, TextAlign align, a3vec3 viewportPosition = a3vec3());
	float calculateLineHeight(int line);
};

#endif