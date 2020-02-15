#ifndef SERVER_H_
#define SERVER_H_

#include <A3_DEMO/_andrick_Utils/andrick_common.h>

class Server
{
public:
	Server() = default;
	~Server() = default;

private:
	static UserId sUserIdCounter;
};

#endif