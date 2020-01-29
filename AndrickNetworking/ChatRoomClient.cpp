#include "ChatRoomClient.h"
#include "User.h"
#include "SceneManager.h"

std::shared_ptr<ChatRoomClient> ChatRoomClient::spInstance = nullptr;

bool ChatRoomClient::isInitialized()
{
	return (spInstance != nullptr);
}

bool ChatRoomClient::initChatRoom(bool isHost, const std::string& serverIP, const std::string& hostUsername)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<ChatRoomClient>(isHost, serverIP, hostUsername);
	}

	return isInitialized();
}

ChatRoomClient::ChatRoomClient(bool isHost, const std::string& serverIP, const std::string& username) :
	mServerIP(serverIP),
	mUsername(username),
	mIsHost(isHost),
	mpClient(nullptr),
	mUsernameMap(std::map<UserId, std::string>()),
	mpPeer(RakNet::RakPeerInterface::GetInstance()),
	mpPacket(nullptr)
{
	mpPeer->Startup(1, &mSocketDescriptor, 1);
}

void ChatRoomClient::update()
{
	receivePacket();
}

//Always looping to get new packet data
void ChatRoomClient::receivePacket()
{
	//Incoming packets to client from server.
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			ChatRoomScene::printMessageToChatRoom("Our connection request has been accepted.");
			requestToJoinServer();
			break;
		}
		case PacketEventId::SET_AUTHORITY:
			break;

		case PacketEventId::DELIVER_PUBLIC_MESSAGE:
		{
			DeliverPublicMessagePacket* data = (DeliverPublicMessagePacket*)(mpPacket->data);

			//TODO: Call message to print to console.
			//std::cout << data->message << std::endl;
			ChatRoomScene::printMessageToChatRoom(data->message);

			break;
		}
		case PacketEventId::DELIVER_PRIVATE_MESSAGE:
		{
			DeliverPrivateMessagePacket* data = (DeliverPrivateMessagePacket*)(mpPacket->data);

			ChatRoomScene::printMessageToChatRoom(data->message);
			break;
		}
		case PacketEventId::JOIN_ACCEPTED:
		{
			JoinAcceptedPacket* joinAcceptedPacket = (JoinAcceptedPacket*)mpPacket->data;

			mHostAddress = mpPacket->systemAddress;
			mpClient = std::make_unique<User>(joinAcceptedPacket->userId, joinAcceptedPacket->username, AuthorityId::NORMAL, mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID()));
			mMaxUsers = joinAcceptedPacket->maxUserCount;
			addUserIdToMap(joinAcceptedPacket->userId, joinAcceptedPacket->username);

			//TODO: Send private message from server to user

			break;
		}
		case PacketEventId::USER_JOINED_SERVER:
		{
			UserJoinedServerPacket* userJoinedPacket = (UserJoinedServerPacket*)(mpPacket->data);

			addUserIdToMap(userJoinedPacket->userId, userJoinedPacket->username);
			//ChatRoomScene::printMessageToChatRoom("There be a USER in this chat!");
			break;
		}
		case PacketEventId::USER_LEFT_SERVER:
		{
			UserLeftServerPacket* userLeftPacket = (UserLeftServerPacket*)(mpPacket->data);

			std::string username = mUsernameMap.find(userLeftPacket->userId)->second;

			removeUserFromMap(userLeftPacket->userId);

			ChatRoomScene::printMessageToChatRoom(username + " left the server.");

			break;
		}
		case PacketEventId::SERVER_CLOSING:
		{
			ChatRoomScene::printMessageToChatRoom("The server closed. Press ESC to return to the main menu.");
			break;
		}
		case PacketEventId::MUTE_USER:
			break;

		case PacketEventId::UNMUTE_USER:
			break;

		default:
			ChatRoomScene::printMessageToChatRoom("Yo, ~I just got a packet, ~I just got a packet! ~I just got a packet, ~don't know; ~what it's; ~from!");
			break;
		}
	}
}

void ChatRoomClient::sendPublicMessage(const std::string& message)
{
	SendPublicMessageRequestPacket messagePacket = SendPublicMessageRequestPacket(mpClient->getUserId(), message);

	mpPeer->Send((const char*)(&messagePacket), sizeof(SendPublicMessageRequestPacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mHostAddress, false);
}

void ChatRoomClient::sendPrivateMessageRequest(const std::string& message, const std::string& toUserName)
{
	//Send packet 
	SendPrivateMessageRequestPacket messagePacket = SendPrivateMessageRequestPacket(mpClient->getUserId(), toUserName, message);

	mpPeer->Send((const char*)(&messagePacket), sizeof(SendPrivateMessageRequestPacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mHostAddress, false);
}

bool ChatRoomClient::connectToServer()
{
	RakNet::ConnectionAttemptResult result = mpPeer->Connect(mServerIP.c_str(), sPORT, 0, 0);
	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

void ChatRoomClient::leaveServer()
{
	//Send a leaving packet.
	UserLeftServerPacket userLeavingPacket = UserLeftServerPacket(
		mpClient->getUserId()
	);

	mpPeer->Send((const char*)(&userLeavingPacket), sizeof(UserLeftServerPacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mHostAddress, false);


	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
	spInstance = nullptr;
	SceneManager::switchScene(SceneId::MAIN_MENU);
}

void ChatRoomClient::requestToJoinServer()
{
	ChatRoomScene::printMessageToChatRoom("Requesting to join server...");

	//We are a client connecting.
	RequestJoinServerPacket requestJoinPacket = RequestJoinServerPacket(
		ChatRoomClient::spInstance->mUsername
	);

	requestJoinPacket.username[ChatRoomClient::spInstance->mUsername.length()] = '\0';

	//ChatRoomClient::spInstance->sendPacket(*requestJoinPacket);
	mpPeer->Send((const char*)(&requestJoinPacket), sizeof(RequestJoinServerPacket), PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
}

void ChatRoomClient::addUserIdToMap(UserId userId, char name[sMAX_USERNAME_LENGTH])
{
	mUsernameMap.insert({ userId, name });
}

void ChatRoomClient::removeUserFromMap(UserId userId)
{
	auto iter = mUsernameMap.find(userId);
	if (iter != mUsernameMap.end())
	{
		mUsernameMap.erase(userId);
	}
}

void ChatRoomClient::initUsernameMap(char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1], int connectedUsers)
{
	mUsernameMap.clear();

	for (int userIndex = 0; userIndex < connectedUsers; ++userIndex)
	{
		char id;
		char username[sMAX_USERNAME_LENGTH];

		for (int usernameIndex = 0; usernameIndex < sMAX_USERNAME_LENGTH; ++usernameIndex)
		{
			//We're at the id spot
			if (usernameIndex == 0)
			{
				id = userInfo[userIndex][0];
			}
			//We're at the start of the username
			else if (usernameIndex == 1)
			{
				memcpy(username, userInfo[usernameIndex], sMAX_USERNAME_LENGTH);
				mUsernameMap.insert({ id, std::string(username) });
				continue;
			}
		}
	}
}
