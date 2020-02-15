#ifndef ANDRICK_CHATLOG_H_
#define ANDRICK_CHATLOG_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

struct LogInfo
{
	MessageType type;
	std::string text;
	Color color = AndrickColors::WHITE;
};

class ChatLog
{
public:
	explicit ChatLog(const unsigned int linesToDisplay);
	virtual ~ChatLog() = default;

	ChatLog(const ChatLog& scene) = delete;
	ChatLog& operator=(const ChatLog& scene) = delete;

	std::shared_ptr<LogInfo> begin();
	std::shared_ptr<LogInfo> end();

	void clearChatLog();

	const LogInfo& getLogAt(std::size_t line);
	void append(MessageType type, const std::string& msg, Color color = AndrickColors::WHITE, unsigned int newLineAmount = 1);
	void append(std::shared_ptr<LogInfo> logInfo, unsigned int newLineAmount = 1);

private:
	unsigned int mLinesToDisplay;
	std::shared_ptr<LogInfo> mpCurrentLine;
	std::vector<std::shared_ptr<LogInfo>> mChatLog;
};

#endif