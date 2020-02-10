#include "andrick_client.h"
#include "../_andrick_Utils/_andrick_text_formatter.h"
#include "../_andrick_Scene/andrick_scene_manager.h"
#include "../_andrick_Demostate/andrick_demostate.h"

std::shared_ptr<Client> Client::spInstance = nullptr;

bool Client::isInitialized()
{
	return (spInstance != nullptr);
}

bool Client::initChatRoom(bool isHost, const std::string& serverIP, const std::string& hostUsername)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<Client>(isHost, serverIP, hostUsername);
	}

	return isInitialized();
}

Client::Client(bool isHost, const std::string& serverIP, const std::string& username) :
	mServerIP(serverIP),
	mUsername(username),
	mIsHost(isHost),
	mpClient(nullptr),
	mUsernameMap(std::map<UserId, std::string>()),
	mpPeer(RakNet::RakPeerInterface::GetInstance()),
	mpPacket(nullptr),
	mMaxUsers()
{
	mpPeer->Startup(1, &mSocketDescriptor, 1);
}

void Client::update(const a3_DemoState* demoState)
{
	receivePacket(demoState);
}

//Always looping to get new packet data
void Client::receivePacket(const a3_DemoState* demoState)
{
	//Incoming packets to client from server.
	for (mpPacket = mpPeer->Receive(); mpPacket; /*mpPeer->DeallocatePacket(mpPacket),*/ mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Our connection request has been accepted.");
			requestToJoinServer(demoState);
			break;
		}
		case PacketEventId::SET_AUTHORITY:
			break;

		case PacketEventId::DELIVER_PUBLIC_MESSAGE:
		{
			DeliverPublicMessagePacket* data = (DeliverPublicMessagePacket*)(mpPacket->data);

			//TODO: Call message to print to console.
			//std::cout << data->message << std::endl;
			demoState->mpSceneManager->mpCurrentScene->addToChatList((MessageType)data->msgType, std::string(data->message));
			break;
		}
		case PacketEventId::DELIVER_PRIVATE_MESSAGE:
		{
			DeliverPrivateMessagePacket* data = (DeliverPrivateMessagePacket*)(mpPacket->data);

			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, std::string(data->message));
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

			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, username + " left the server.");
			break;
		}
		case PacketEventId::SERVER_CLOSING:
		{
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "The server closed. Press ESC to return to the main menu.");
			break;
		}
		case PacketEventId::MUTE_USER:
			break;

		case PacketEventId::UNMUTE_USER:
			break;

		default:
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Yo, ~I just got a packet, ~I just got a packet! ~I just got a packet, ~don't know; ~what it's; ~from!");
			break;
		}
	}
}

void Client::sendPublicMessage(const std::string& message, MessageType type)
{
	SendPublicMessageRequestPacket messagePacket = SendPublicMessageRequestPacket(mpClient->getUserId(), message, type);

	mpPeer->Send((const char*)(&messagePacket), sizeof(SendPublicMessageRequestPacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mHostAddress, false);
}

void Client::sendPrivateMessageRequest(const std::string& message, const std::string& toUserName)
{
	//Send packet 
	SendPrivateMessageRequestPacket messagePacket = SendPrivateMessageRequestPacket(mpClient->getUserId(), toUserName, message);

	mpPeer->Send((const char*)(&messagePacket), sizeof(SendPrivateMessageRequestPacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, mHostAddress, false);
}

bool Client::connectToServer()
{
	RakNet::ConnectionAttemptResult result = mpPeer->Connect(mServerIP.c_str(), sPORT, 0, 0);
	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

void Client::leaveServer()
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
}

void Client::requestToJoinServer(const a3_DemoState* demoState)
{
	demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Requesting to join server...");

	//We are a client connecting.
	RequestJoinServerPacket requestJoinPacket = RequestJoinServerPacket(
		Client::spInstance->mUsername
	);

	requestJoinPacket.username[Client::spInstance->mUsername.length()] = '\0';

	//Client::spInstance->sendPacket(*requestJoinPacket);
	mpPeer->Send((const char*)(&requestJoinPacket), sizeof(RequestJoinServerPacket), PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
}

void Client::addUserIdToMap(UserId userId, char name[sMAX_USERNAME_LENGTH])
{
	mUsernameMap.insert({ userId, name });
}

void Client::removeUserFromMap(UserId userId)
{
	auto iter = mUsernameMap.find(userId);
	if (iter != mUsernameMap.end())
	{
		mUsernameMap.erase(userId);
	}
}

void Client::initUsernameMap(char userInfo[sMAX_USERS][sMAX_USERNAME_LENGTH + 1], int connectedUsers)
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
