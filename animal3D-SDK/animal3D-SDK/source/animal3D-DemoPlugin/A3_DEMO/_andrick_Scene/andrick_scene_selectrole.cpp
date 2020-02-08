#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"
#include "andrick_scene_selectrole.h"

SelectRoleScene::SelectRoleScene() :
	Scene(SceneId::SelectRole),
	mCurrentStep(SelectRoleStep::IS_CLIENT_OR_HOST),
	mIsHost(false)
{

}

void SelectRoleScene::input(const a3_DemoState* demoState)
{
	if (mCurrentStep == SelectRoleStep::IS_CLIENT_OR_HOST)
	{
		//Host
		if (a3keyboardIsChanged(demoState->keyboard, a3key_H) > 0)
		{
			mIsHost = true;
			mCurrentStep = SelectRoleStep::HOST_ENTER_USERNAME;
		}

		//Client
		if (a3keyboardIsChanged(demoState->keyboard, a3key_C) > 0)
		{
			mIsHost = false;
			mCurrentStep = SelectRoleStep::CLIENT_ENTER_IP;
		}
	}
	else if (mCurrentStep == SelectRoleStep::HOST_ENTER_USERNAME)
	{
		if (a3keyboardIsChanged(demoState->keyboard, (a3_KeyboardKey)demoState->currentKey) > 0)
		{
			if (demoState->currentKey == a3key_enter)
			{
				//mCurrentInput.clear();
				mCurrentStep = SelectRoleStep::HOST_MAX_CLIENTS;
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
		if (a3keyboardIsChanged(demoState->keyboard, (a3_KeyboardKey)demoState->currentKey) > 0)
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
		int userCount;
		try
		{
			userCount = std::stoi(mCurrentInput);
		}
		catch (...)
		{
			//Please enter a valid number!
			userCount = sDEFAULT_MAX_USERS;
		}

		//GO TO LOBBY SCENE
		demoState->mpSceneManager->switchToScene(SceneId::Tictactoe);
	}
}

void SelectRoleScene::render(const a3_DemoState* demoState)
{
	glClearColor(0.0f, 0.5f, 2.0f, 1.0f);
	
	TextFormatter::get().setLine(0);

	if (mCurrentStep == SelectRoleStep::IS_CLIENT_OR_HOST)
	{
		TextFormatter::get().drawText(demoState, "Host(H) or Client(C)?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
	}
	else if (mCurrentStep == SelectRoleStep::HOST_ENTER_USERNAME)
	{
		TextFormatter::get().drawText(demoState, "Username?", TextFormatter::WHITE);
		TextFormatter::get().offsetLine(2);
		TextFormatter::get().drawText(demoState, mCurrentInput, TextFormatter::WHITE);
	}
	else if (mCurrentStep == SelectRoleStep::HOST_MAX_CLIENTS)
	{
		TextFormatter::get().drawText(demoState, "Max Clients?", TextFormatter::WHITE);
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