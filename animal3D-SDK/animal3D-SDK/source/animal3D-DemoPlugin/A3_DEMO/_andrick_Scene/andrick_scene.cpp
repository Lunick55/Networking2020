#include "andrick_scene.h"
#include "A3_DEMO/_andrick_Demostate/andrick_demostate.h"

Scene::Scene(const SceneId id) :
	mId(id)
{

}

const SceneId Scene::getId() const 
{ 
	return mId; 
}

void Scene::addToChatList(MessageType type, const std::string& msg, unsigned int newLineAmount)
{
	LogInfo info;
	info.type = type;
	info.text = msg;
	addToChatList(info, newLineAmount);
}

void Scene::addToChatList(LogInfo logInfo, unsigned int newLineAmount)
{
	mChatLog.emplace_back(logInfo);

	LogInfo spacer;
	spacer.text = "\n";
	spacer.type = logInfo.type;

	unsigned int i = 0;
	while (i < newLineAmount)
	{
		i++;
		mChatLog.emplace_back(spacer);
	}
}

bool Scene::isKeyPressed(const a3_DemoState* demoState, a3_KeyboardKey key)
{
	return a3keyboardIsChanged(demoState->keyboard, key) > 0;
}