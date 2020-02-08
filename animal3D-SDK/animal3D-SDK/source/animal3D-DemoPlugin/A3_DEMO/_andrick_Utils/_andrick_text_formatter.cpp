#include "_andrick_text_formatter.h"
#include "../_andrick_Demostate/andrick_demostate.h"

const float TextFormatter::SCREEN_LEFT = -0.98f;
const float TextFormatter::SCREEN_RIGHT = 0.98f;
const float TextFormatter::SCREEN_TOP = 0.98f;
const float TextFormatter::SCREEN_BOTTOM = -0.98f;
const float TextFormatter::SCREEN_CENTER = 0.0f;
const float TextFormatter::LINE_HEIGHT = 0.08f;
const float TextFormatter::LETTER_SIZE = 0.022f;

a3vec4 TextFormatter::WHITE = createColor(1.0f, 1.0f, 1.0f, 1.0f);

a3vec4 TextFormatter::createColor(float r, float g, float b, float a)
{
	a3vec4 color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	return color;
}

TextFormatter::TextFormatter() :
	mColor(WHITE),
	mAlignment(TextAlign::LEFT)
{
	setLine(0);
	mCursorPosition.x = SCREEN_LEFT;
}

void TextFormatter::setColor(a3vec4 color)
{
	mColor = color;
}


void TextFormatter::setAlignment(TextAlign align)
{
	mAlignment = align;
}

void TextFormatter::setCursorPos(float x, float y, float z)
{
	mCursorPosition.x = x;
	mCursorPosition.y = y;
	mCursorPosition.z = z;
}

void TextFormatter::drawText(const a3_DemoState* demoState, const std::string& string, TextAlign alignOverride)
{
	drawText(demoState, string, mColor, alignOverride);
}

void TextFormatter::drawText(const a3_DemoState* demoState, const std::string& string, a3vec4 color, TextAlign alignOverride, a3vec3 viewportPosition)
{
	a3vec3 position = mCursorPosition;

	if (alignOverride == TextAlign::NONE)
	{
		position = calculatePosition(string, mAlignment);
	}
	else
	{
		position = calculatePosition(string, alignOverride, viewportPosition);
	}

	a3textDraw(demoState->text, position.x, position.y, position.z, color.r, color.g, color.b, 1.0f, string.c_str());
}

void TextFormatter::newLine()
{
	++mCurrentLine;
	setCursorPos(mCursorPosition.x, calculateLineHeight(mCurrentLine), mCursorPosition.z);
}

void TextFormatter::setLine(int lineNumber)
{
	mCurrentLine = lineNumber;
	setCursorPos(mCursorPosition.x, calculateLineHeight(mCurrentLine), mCursorPosition.z);
}

void TextFormatter::offsetLine(int offset)
{
	mCurrentLine += offset;
	setCursorPos(mCursorPosition.x, calculateLineHeight(mCurrentLine), mCursorPosition.z);
}

a3vec3 TextFormatter::calculatePosition(const std::string string, TextAlign align, a3vec3 viewportPosition)
{
	a3vec3 position = mCursorPosition;

	switch (align)
	{
	case TextAlign::LEFT:
		position.x = SCREEN_LEFT;
		break;
	case TextAlign::RIGHT:
		position.x = SCREEN_RIGHT - (string.size() * LETTER_SIZE);
		break;
	case TextAlign::CENTER_X:
	{
		position.x = SCREEN_CENTER - (string.size() * LETTER_SIZE * 0.5f);
		break;
	}
	case TextAlign::CENTER_Y:
	{
		position.y = SCREEN_CENTER;
		break;
	}
	case TextAlign::CENTER:
	{
		position.x = SCREEN_CENTER - (string.size() * LETTER_SIZE * 0.5f);
		position.y = SCREEN_CENTER;
		break;
	}
	case TextAlign::UNALIGN:
		position = viewportPosition;
		break;
	default:
		break;
	}

	return position;
}

float TextFormatter::calculateLineHeight(int line)
{
	float lineHeight = 1.0f - LINE_HEIGHT;
	lineHeight -= mCurrentLine * LINE_HEIGHT;
	return lineHeight;
}