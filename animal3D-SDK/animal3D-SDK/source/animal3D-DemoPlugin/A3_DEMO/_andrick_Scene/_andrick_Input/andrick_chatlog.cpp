#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

ChatLog::ChatLog(const unsigned int linesToDisplay) :
	mLinesToDisplay(linesToDisplay),
	mpCurrentLine(nullptr)
{

}

std::shared_ptr<LogInfo> ChatLog::begin()
{
	return *mChatLog.begin();
}

std::shared_ptr<LogInfo> ChatLog::end()
{
	return *mChatLog.end();
}

void ChatLog::clearChatLog()
{
	mChatLog.clear();
	mpCurrentLine = nullptr;
}

const LogInfo& ChatLog::getLogAt(std::size_t line)
{
	assert(line >= 0 && line < mChatLog.size());
	return *mChatLog.at(line);
}

void ChatLog::append(MessageType type, const std::string& msg, Color color, unsigned int newLineAmount)
{
	std::shared_ptr<LogInfo> info = std::make_shared<LogInfo>();
	info->type = type;
	info->text = msg;
	info->color = color;
	append(info, newLineAmount);
}

void ChatLog::append(std::shared_ptr<LogInfo> logInfo, unsigned int newLineAmount)
{
	mChatLog.emplace_back(logInfo);

	std::shared_ptr<LogInfo> spacer = std::make_shared<LogInfo>();
	spacer->text = "\n";
	spacer->type = logInfo->type;

	unsigned int i = 0;
	while (i < newLineAmount)
	{
		i++;
		mChatLog.emplace_back(spacer);
	}
}
