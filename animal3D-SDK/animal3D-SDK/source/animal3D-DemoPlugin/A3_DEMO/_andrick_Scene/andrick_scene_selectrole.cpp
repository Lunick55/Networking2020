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

void SelectRoleScene::enteringScene(const a3_DemoState* demoState) 
{

}

void SelectRoleScene::input(a3_DemoState* demoState)
{
	if (mCurrentStep == SelectRoleStep::IS_CLIENT_OR_HOST)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			demoState->exitFlag = 1;
			return;
		}
		else if (isKeyPressed(demoState, a3key_H))							//Host
		{
			mIsHost = true;
			mCurrentStep = SelectRoleStep::HOST_ENTER_USERNAME;
		}
		else if (isKeyPressed(demoState, a3key_C))						//Client
		{
			mIsHost = false;
			mCurrentStep = SelectRoleStep::CLIENT_ENTER_USERNAME;
		}
	}
	else if (mCurrentStep == SelectRoleStep::HOST_ENTER_USERNAME)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = SelectRoleStep::IS_CLIENT_OR_HOST;
			return;
		}
		else if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
		{
			if (demoState->currentKey == a3key_enter)
			{
				if (!mCurrentInput.empty())
				{
					mCurrentStep = SelectRoleStep::HOST_MAX_CLIENTS;
					mUsername = mCurrentInput;
					mCurrentInput.clear();
				}
			}
			else if (demoState->currentKey == a3key_backspace && mCurrentInput.size() > 0)
			{
				mCurrentInput = mCurrentInput.substr(0, mCurrentInput.size() - 1);
			}
			else
			{
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	else if (mCurrentStep == SelectRoleStep::HOST_MAX_CLIENTS)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = SelectRoleStep::HOST_ENTER_USERNAME;
			return;
		}
		else if (a3keyboardIsChanged(demoState->keyboard, (a3_KeyboardKey)demoState->currentKey) > 0)
		{
			if (demoState->currentKey == a3key_enter)
			{
				mCurrentStep = SelectRoleStep::HOST_VALIDATE_MAX_CLIENTS;
			}
			else if (demoState->currentKey == a3key_backspace && mCurrentInput.size() > 0)
			{
				mCurrentInput = mCurrentInput.substr(0, mCurrentInput.size() - 1);
			}
			else
			{
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	else if (mCurrentStep == SelectRoleStep::CLIENT_ENTER_USERNAME)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = SelectRoleStep::IS_CLIENT_OR_HOST;
			return;
		}
		else if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
		{
			if (demoState->currentKey == a3key_enter)
			{
				if (!mCurrentInput.empty())
				{
					mCurrentStep = SelectRoleStep::CLIENT_ENTER_IP;
					mUsername = mCurrentInput;
					mCurrentInput.clear();
				}
			}
			else if (demoState->currentKey == a3key_backspace && mCurrentInput.size() > 0)
			{
				mCurrentInput = mCurrentInput.substr(0, mCurrentInput.size() - 1);
			}
			else
			{
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
	else if (mCurrentStep == SelectRoleStep::CLIENT_ENTER_IP)
	{
		if (isKeyPressed(demoState, a3key_escape))
		{
			mCurrentStep = SelectRoleStep::CLIENT_ENTER_USERNAME;
			return;
		}
		else if (isKeyPressed(demoState, (a3_KeyboardKey)demoState->currentKey))
		{
			if (demoState->currentKey == a3key_enter)
			{
				if (!mCurrentInput.empty())
				{
					mIP = mCurrentInput;
					mCurrentInput.clear();
					mCurrentStep = SelectRoleStep::CLIENT_VALIDATE_IP;
				}
			}
			else if (demoState->currentKey == a3key_backspace && mCurrentInput.size() > 0)
			{
				mCurrentInput = mCurrentInput.substr(0, mCurrentInput.size() - 1);
			}
			else if (demoState->currentKey == 190)
			{
				mCurrentInput += ".";
			}
			else
			{
				mCurrentInput += (char)(demoState->currentKey);
			}
		}
	}
}

void SelectRoleScene::networkReceive(const a3_DemoState* demoState)
{
	
}

void SelectRoleScene::update(const a3_DemoState* demoState)
{
	if (mCurrentStep == SelectRoleStep::HOST_ENTER_USERNAME)
	{
		if (mCurrentInput.size() > sMAX_USERNAME_LENGTH)
		{
			mCurrentInput = mCurrentInput.substr(0, sMAX_USERNAME_LENGTH);
		}
	}
	else if (mCurrentStep == SelectRoleStep::HOST_VALIDATE_MAX_CLIENTS)
	{
		int maxUsers;
		try
		{
			maxUsers = std::stoi(mCurrentInput);
		}
		catch (...)
		{
			//Please enter a valid number!
			mCurrentInput.clear();
			maxUsers = sDEFAULT_MAX_USERS;
			//mCurrentStep = HOST_MAX_CLIENTS;
			return;
		}

		//INIT CHAT ROOM AND GO TO LOBBY SCENE
		if (Client::initChatRoom(true, "127.0.0.1", mUsername) && Host::initChatRoom(sPORT, maxUsers, mUsername))
		{
			demoState->mpSceneManager->switchToScene(demoState, SceneId::Lobby);
		}
	}
	else if (mCurrentStep == SelectRoleStep::CLIENT_VALIDATE_IP)
	{
		//INIT CHAT ROOM AND GO TO LOBBY SCENE
		if (Client::initChatRoom(false, mIP, mUsername))
		{
			demoState->mpSceneManager->switchToScene(demoState, SceneId::Lobby);
		}
	}
}

void SelectRoleScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.0f, 0.5f, 2.0f, 1.0f);

	TextFormatter::get().setLine(0);
	TextFormatter::get().drawText(demoState, "Select Role Scene", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	TextFormatter::get().offsetLine(2);

	if (mCurrentStep == SelectRoleStep::IS_CLIENT_OR_HOST)
	{
		TextFormatter::get().drawText(demoState, "Host(H) or Client(C)?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
	}
	else if (mCurrentStep == SelectRoleStep::HOST_ENTER_USERNAME)
	{
		TextFormatter::get().drawText(demoState, "What do you want your username to be?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}
	else if (mCurrentStep == SelectRoleStep::HOST_MAX_CLIENTS)
	{
		TextFormatter::get().drawText(demoState, "Max Clients?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}
	else if (mCurrentStep == SelectRoleStep::HOST_VALIDATE_MAX_CLIENTS)
	{
		TextFormatter::get().drawText(demoState, "Please enter a valid number!", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}
	else if (mCurrentStep == SelectRoleStep::CLIENT_ENTER_USERNAME)
	{
		TextFormatter::get().drawText(demoState, "What do you want your username to be?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}
	else if (mCurrentStep == SelectRoleStep::CLIENT_ENTER_IP)
	{
		TextFormatter::get().drawText(demoState, "What is the server ip?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}

	//TextFormatter::get().setLine(0);
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE);
	//	TextFormatter::get().newLine();
	//}
	//
	//TextFormatter::get().setLine(0);
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE, TextFormatter::TextAlign::RIGHT);
	//	TextFormatter::get().newLine();
	//}
	//
	//TextFormatter::get().setLine(0);
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::WHITE, TextFormatter::TextAlign::CENTER_X);
	//	TextFormatter::get().newLine();
	//}
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	a3vec3 randPos;
	//	randPos.x = ((rand() % 100) / 50.0f) - 1.0f;
	//	randPos.y = ((rand() % 100) / 50.0f) - 1.0f;
	//	randPos.z = -1.0f;
	//	TextFormatter::get().drawText(demoState, "Hello, world!", TextFormatter::createColor(1.0f, 0.0, 0.0, 1.0f), TextFormatter::TextAlign::UNALIGN, randPos);
	//}
}


void SelectRoleScene::networkSend(const a3_DemoState* demoState)
{

}