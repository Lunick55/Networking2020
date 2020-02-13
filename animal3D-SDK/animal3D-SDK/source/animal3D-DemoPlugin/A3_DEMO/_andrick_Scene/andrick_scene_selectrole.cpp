#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_selectrole.h"
#include "../_andrick_Network/andrick_host.h"

SelectRoleScene::SelectRoleScene() :
	Scene(SceneId::SelectRole),
	mCurrentStep(SelectRoleStep::IS_CLIENT_OR_HOST),
	mUsername(),
	mIsHost(false)
{

}

//Initializing scene on entry
void SelectRoleScene::enteringScene(const a3_DemoState* demoState) 
{
	Scene::enteringScene(demoState);

	mCurrentStep = SelectRoleStep::IS_CLIENT_OR_HOST;
	mIsHost = false;
}

//Input handling
void SelectRoleScene::input(a3_DemoState* demoState)
{
	switch (mCurrentStep)
	{
	case SelectRoleStep::IS_CLIENT_OR_HOST:
		handleInputIsHostOrClient(demoState);
		break;
	case SelectRoleStep::CLIENT_ENTER_USERNAME:
		handleInputClientEnterUsername(demoState);
		break;
	case SelectRoleStep::CLIENT_ENTER_IP:
		handleInputClientEnterIP(demoState);
		break;
	case SelectRoleStep::CLIENT_CONNECT_TO_SERVER:
		handleInputClientConnectToServer(demoState);
		break;
	case SelectRoleStep::HOST_ENTER_USERNAME:
		handleInputHostEnterUsername(demoState);
		break;
	case SelectRoleStep::HOST_MAX_CLIENTS:
		handleInputHostMaxClients(demoState);
		break;
	case SelectRoleStep::HOST_INVALID_MAX_CLIENTS:
		handleInputHostInvalidMaxClients(demoState);
		break;
	case SelectRoleStep::INVALID_USERNAME:
		handleInputInvalidUsername(demoState);
		break;
	default:
		return;
	}
}
bool SelectRoleScene::handleInputEscape(a3_DemoState* demoState, SelectRoleStep targetStep)
{
	if (isKeyPressed(demoState, a3key_escape))
	{
		switch (targetStep)
		{
		case SelectRoleStep::CLOSE_PROGRAM:
			demoState->exitFlag = 1;
		default:
			mCurrentStep = targetStep;
		}

		return true;
	}

	return false;
}
void SelectRoleScene::handleInputIsHostOrClient(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, SelectRoleStep::CLOSE_PROGRAM)) return;

	if (isKeyPressed(demoState, a3key_1))
	{
		mIsHost = true;
		mCurrentStep = SelectRoleStep::HOST_ENTER_USERNAME;
	}
	else if (isKeyPressed(demoState, a3key_2))
	{
		mIsHost = false;
		mCurrentStep = SelectRoleStep::CLIENT_ENTER_USERNAME;
	}
}
void SelectRoleScene::handleInputClientEnterUsername(a3_DemoState* demoState)
{
	handleInputUsername(demoState, SelectRoleStep::CLIENT_ENTER_IP);
}
void SelectRoleScene::handleInputClientEnterIP(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, SelectRoleStep::CLIENT_ENTER_USERNAME)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!mCurrentInput.empty())
		{
			mIP = mCurrentInput;
			mCurrentInput.clear();
			mCurrentStep = SelectRoleStep::CLIENT_CONNECT_TO_SERVER;
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void SelectRoleScene::handleInputClientConnectToServer(a3_DemoState* demoState)
{
	if (Client::initChatRoom(false, mIP, mUsername))
	{
		demoState->mpSceneManager->switchToScene(demoState, SceneId::Lobby);
	}
	else
	{
		if (handleInputEscape(demoState, SelectRoleStep::CLIENT_ENTER_USERNAME)) return;
	}
}
void SelectRoleScene::handleInputHostEnterUsername(a3_DemoState* demoState)
{
	handleInputUsername(demoState, SelectRoleStep::HOST_MAX_CLIENTS);
}
void SelectRoleScene::handleInputHostMaxClients(a3_DemoState* demoState)
{
	if (handleInputEscape(demoState, SelectRoleStep::HOST_ENTER_USERNAME)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		int maxUsers;
		if (!validateNumber(mCurrentInput, maxUsers))
		{
			mCurrentStep = SelectRoleStep::HOST_INVALID_MAX_CLIENTS;
		}
		else
		{
			if (Client::initChatRoom(true, "127.0.0.1", mUsername) && Host::initChatRoom(sPORT, maxUsers, mUsername))
			{
				demoState->mpSceneManager->switchToScene(demoState, SceneId::Lobby);
			}
		}
	}
	else
	{
		Scene::input(demoState);
	}
}
void SelectRoleScene::handleInputHostInvalidMaxClients(a3_DemoState* demoState)
{
	handleInputHostMaxClients(demoState);
}
void SelectRoleScene::handleInputInvalidUsername(a3_DemoState* demoState)
{
	if (mIsHost)
	{
		handleInputUsername(demoState, SelectRoleStep::HOST_MAX_CLIENTS);
	}
	else
	{
		handleInputUsername(demoState, SelectRoleStep::CLIENT_ENTER_IP);
	}
}
void SelectRoleScene::handleInputUsername(a3_DemoState* demoState, SelectRoleStep nextStep)
{
	if (handleInputEscape(demoState, SelectRoleStep::IS_CLIENT_OR_HOST)) return;

	if (isKeyPressed(demoState, a3key_enter))
	{
		if (!validateUsername(mCurrentInput))
		{
			mCurrentStep = SelectRoleStep::INVALID_USERNAME;
		}
		else
		{
			mUsername = mCurrentInput;
			mCurrentInput.clear();
			mCurrentStep = nextStep;
		}
	}
	else
	{
		Scene::input(demoState);
		std::cout << mCurrentInput << std::endl;
	}
}
//void SelectRoleScene::handleInvalidInput(a3_DemoState* demoState)
//{
//	if (handleInputEscape(demoState, SelectRoleStep::IS_CLIENT_OR_HOST)) return;
//
//	Scene::handleTyping(demoState, mCurrentInput);
//}

//Incoming packet handling. Empty because we are not connected to a server in this scene
void SelectRoleScene::networkReceive(const a3_DemoState* demoState)
{
}

//Update handling. Empty because everything is input based in this scene
void SelectRoleScene::update(const a3_DemoState* demoState)
{
}

//Render handling
void SelectRoleScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.0f, 0.5f, 2.0f, 1.0f);

	TextFormatter& textFormatter = TextFormatter::get();
	textFormatter.setAlignment(TextFormatter::TextAlign::CENTER_X);

	TextFormatter::get().setLine(0);
	TextFormatter::get().drawText(demoState, "Select Role Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	TextFormatter::get().offsetLine(2);

	switch (mCurrentStep)
	{
	case SelectRoleStep::IS_CLIENT_OR_HOST:
		renderIsHostOrClient(demoState, textFormatter);
		break;
	case SelectRoleStep::CLIENT_ENTER_USERNAME:
		renderClientEnterUsername(demoState, textFormatter);
		break;
	case SelectRoleStep::CLIENT_ENTER_IP:
		renderClientEnterIP(demoState, textFormatter);
		break;
	case SelectRoleStep::CLIENT_CONNECT_TO_SERVER:
		renderClientConnectToServer(demoState, textFormatter);
		break;
	case SelectRoleStep::HOST_ENTER_USERNAME:
		renderHostEnterUsername(demoState, textFormatter);
		break;
	case SelectRoleStep::HOST_MAX_CLIENTS:
		renderHostMaxClients(demoState, textFormatter);
		break;
	case SelectRoleStep::HOST_INVALID_MAX_CLIENTS:
		renderHostInvalidMaxClients(demoState, textFormatter);
		break;
	case SelectRoleStep::INVALID_USERNAME:
		renderInvalidUsername(demoState, textFormatter);
		break;
	default:
		return;
	}
}

void SelectRoleScene::renderIsHostOrClient(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "Welcome! Select an option from the menu", TextFormatter::WHITE);
	formatter.offsetLine(2);
	formatter.drawText(demoState, "(1) Host a server", TextFormatter::WHITE);
	formatter.newLine();
	formatter.drawText(demoState, "(2) Join a server", TextFormatter::WHITE);
	formatter.offsetLine(2);
}
void SelectRoleScene::renderClientEnterUsername(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "You are a client!", TextFormatter::WHITE);
	formatter.newLine();
	renderUsername(demoState, formatter);
}
void SelectRoleScene::renderClientEnterIP(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "What is the server ip?", TextFormatter::WHITE);
	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE);
}
void SelectRoleScene::renderClientConnectToServer(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "Initializing client...", TextFormatter::WHITE);
}
void SelectRoleScene::renderHostEnterUsername(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "You are the host!", TextFormatter::WHITE);
	formatter.newLine();
	renderUsername(demoState, formatter);
}
void SelectRoleScene::renderHostMaxClients(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "How many clients can the server hold?", TextFormatter::WHITE);
	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE);
}
void SelectRoleScene::renderHostInvalidMaxClients(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "Please enter a valid number!", TextFormatter::WHITE);
	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE);
}
void SelectRoleScene::renderInvalidUsername(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "That username is invalid!", TextFormatter::RED);
	formatter.newLine();
	formatter.drawText(demoState, "Must be between 1 and " + std::to_string(sMAX_USERNAME_LENGTH) + " chars and no spaces or commas.", TextFormatter::RED);
	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE);
}
void SelectRoleScene::renderUsername(const a3_DemoState* demoState, TextFormatter& formatter)
{
	formatter.drawText(demoState, "What do you want your username to be?", TextFormatter::WHITE);
	formatter.offsetLine(2);
	formatter.drawText(demoState, mCurrentInput, TextFormatter::WHITE);
}

//Outgoing packet handling. Empty because we are not connected to a server in this scene
void SelectRoleScene::networkSend(const a3_DemoState* demoState)
{

}
