#ifndef ANDRICK_CHATLOG_H_
#define ANDRICK_CHATLOG_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

struct LogInfo
{
	LogInfo(MessageType t = MessageType::GLOBAL, const std::string msg = "\n", Color c = WHITE) :
		type(t), text(msg), color(c) {}

	MessageType type;
	std::string text;
	Color color;
};

class ChatLog
{
public:
	explicit ChatLog();
	virtual ~ChatLog() = default;

	ChatLog(const ChatLog& scene) = delete;
	ChatLog& operator=(const ChatLog& scene) = delete;

	std::shared_ptr<LogInfo> begin();
	std::shared_ptr<LogInfo> end();

	void clearChatLog();

	std::shared_ptr<LogInfo> getLogAt(std::size_t line);
	void append(MessageType type, const std::string& msg, Color color = WHITE, unsigned int newLineAmount = 0);
	void append(std::shared_ptr<LogInfo> logInfo, unsigned int newLineAmount = 0);

	const std::vector<std::shared_ptr<LogInfo>> getRecentChatLog(unsigned int offsetFromMostRecent = 0, unsigned int lineHistory = 10);

private:
	std::shared_ptr<LogInfo> mpCurrentLine;
	std::vector<std::shared_ptr<LogInfo>> mChatLog;
};

#endif