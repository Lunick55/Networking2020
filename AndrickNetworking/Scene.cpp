#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(SceneId id) :
	mId(id)
{

}

void Scene::requestQuit()
{
	clearScreen();
	SceneManager::stop();
	std::cout << "Goodbye!" << std::endl;
}

//Might do something with the console later
void Scene::clearInput()
{
	mCurrentInput = "";
}

void Scene::enteringScene()
{
	clearInput();
	drawInitialScene();
	render();
}

//Windows specific.
void Scene::clearScreen()
{
	system("cls");
}

/*
	Returns the console cursor x position.
*/
int Scene::getConsoleCursorX()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo))
	{
		return -1;
	}
	return bufferInfo.dwCursorPosition.X;
}

/*
	Returns the console cursor y position.
*/
int Scene::getConsoleCursorY()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo))
	{
		return -1;
	}

	return bufferInfo.dwCursorPosition.Y;
}

/*
	Returns the console width.
*/
int Scene::getConsoleWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	return columns;
}

/*
	Returns the console height.
*/
int Scene::getConsoleHeight()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return rows;
}

/*
	Sets the cursor to the correct position to draw in the center of the screen depending on the width entered.
*/
void Scene::setCursorForCenterText(int width)
{
	setCursorPosition((getConsoleWidth() - width) / 2, getConsoleCursorY());
}

void Scene::setCursorPosition(std::size_t x, std::size_t y)
{
	std::cout.flush();
	COORD coord = { (short)x, (short)y };
	SetConsoleCursorPosition(windowHandle, coord);
}

/*
	Sets the cursor to the correct position to draw the given string at the center of the width entered.
*/
std::string Scene::generateCenteredText(std::string text, std::size_t width)
{
	if (width < text.length())
	{
		return "TEXT IS TOO LONG FOR CENTERING";
	}

	std::string space;
	space.assign(((width - text.length()) / 2), ' ');
	text = space + text + space;

	if (text.length() < width)
	{
		text = text + " ";
	}
	else if (text.length() > width)
	{
		text.pop_back();
	}

	return text;
}

void Scene::drawCenteredText(const std::string& text, std::size_t width)
{
	std::cout << generateCenteredText(text, width);
	std::cout.flush();
}
/*
	Draws a line across the screen at the given line height.
*/
void Scene::drawLine(int lineHeight)
{
	int width = getConsoleWidth();
	std::string line;
	line.assign(width, '=');

	setCursorPosition(0, lineHeight);
	std::cout << line << std::endl;
}

void Scene::clearScreenPortion(std::size_t startX, std::size_t startY, std::size_t lengthX, std::size_t lengthY)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	if (!GetConsoleScreenBufferInfo(windowHandle, &bufferInfo))
	{
		return;
	}

	std::string clear;
	clear.assign(lengthX, ' ');

	for (std::size_t i = 0; i < lengthY; i++)
	{
		setCursorPosition(startX, startY + i);
		std::cout << clear;
		std::cout.flush();
	}

}