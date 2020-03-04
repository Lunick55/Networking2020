#include "andrick_text_formatter.h"
#include "../_andrick_Demostate/andrick_demostate.h"
#include "A3_DEMO/_andrick_Utils/andrick_common.h"

const float TextFormatter::SCREEN_LEFT = -0.96f;
const float TextFormatter::SCREEN_RIGHT = 0.96f;
const float TextFormatter::SCREEN_TOP = 0.96f;
const float TextFormatter::SCREEN_BOTTOM = -0.96f;
const float TextFormatter::SCREEN_CENTER = 0.0f;
const float TextFormatter::LINE_HEIGHT = 0.08f;
const float TextFormatter::LETTER_SIZE = 0.023f;

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

void TextFormatter::drawText(const std::string& string, TextAlign alignOverride)
{
	drawText(string, mColor, alignOverride);
}

void TextFormatter::drawText(const std::string& string, a3vec4 color, TextAlign alignOverride, a3vec3 viewportPosition)
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

	if (string.compare("\n") == 0)
	{
		newLine();
	}
	else
	{
		a3textDraw(gDemoState->text, position.x, position.y, position.z, color.r, color.g, color.b, 1.0f, string.c_str());
	}

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
	float lineHeight = 1.0f - LINE_HEIGHT - (1.0f - SCREEN_TOP);
	lineHeight -= mCurrentLine * LINE_HEIGHT;
	return lineHeight;
}

std::string TextFormatter::formattedChatMessage(const char* username, const char* message, AuthorityId authority)
{
	std::string formattedMessage = "[";

	if (authority == AuthorityId::ADMIN)
	{
		formattedMessage += "*** ";
	}

	formattedMessage += std::string(username) + "] " + std::string(message);
	return formattedMessage;
}

std::string TextFormatter::formattedChatMessage(const std::string& username, const std::string& message, AuthorityId authority)
{
	return formattedChatMessage(username.c_str(), message.c_str(), authority);
}