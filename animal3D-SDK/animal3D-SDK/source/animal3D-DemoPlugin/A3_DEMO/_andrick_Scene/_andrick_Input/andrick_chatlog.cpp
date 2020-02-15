#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_chatlog.h>

ChatLog::ChatLog() :
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

std::shared_ptr<LogInfo> ChatLog::getLogAt(std::size_t line)
{
	assert(line >= 0 && line < mChatLog.size());
	return mChatLog.at(line);
}

void ChatLog::append(MessageType type, const std::string& msg, Color color, unsigned int newLineAmount)
{
	std::shared_ptr<LogInfo> info = std::make_shared<LogInfo>(type, msg, color);
	append(info, newLineAmount);
}

void ChatLog::append(std::shared_ptr<LogInfo> logInfo, unsigned int newLineAmount)
{
	mChatLog.emplace_back(logInfo);
	
	std::shared_ptr<LogInfo> spacer = std::make_shared<LogInfo>(logInfo->type, "\n", WHITE);
	
	unsigned int i = 0;
	while (i < newLineAmount)
	{
		i++;
		mChatLog.emplace_back(spacer);
	}
	
	mpCurrentLine = mChatLog.back();
}

const std::vector<std::shared_ptr<LogInfo>> ChatLog::getRecentChatLog(unsigned int offsetFromMostRecent, unsigned int lineHistory)
{
	int mostRecentLineIndex = (mChatLog.size()) - offsetFromMostRecent;
	int oldestLineIndex = mostRecentLineIndex - lineHistory;
	
	if (oldestLineIndex < 0)
	{
		oldestLineIndex = 0;
	}
	
	return std::vector<std::shared_ptr<LogInfo>>(mChatLog.begin() + oldestLineIndex, mChatLog.begin() + mostRecentLineIndex);
}