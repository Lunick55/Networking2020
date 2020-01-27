#include "ChatRoomServer.h"
#include "Command.h"
#include "User.h"
#include "SceneManager.h"

std::shared_ptr<ChatRoomServer> ChatRoomServer::spInstance = nullptr;
UserId ChatRoomServer::sNextUniqueId = 0;

bool ChatRoomServer::isInitialized()
{
	return (spInstance != nullptr);
}

bool ChatRoomServer::initChatRoom(const int port, const int maxUsers, const std::string& hostUsername)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<ChatRoomServer>(port, maxUsers, hostUsername);
	}

	return isInitialized();
}

ChatRoomServer::ChatRoomServer(const int port, const int maxUsers, const std::string& hostUsername) :
	mPORT(port), 
	mMAX_USERS(maxUsers), 
	mIsRunning(false),
	mpPeer(RakNet::RakPeerInterface::GetInstance()),
	mpPacket(nullptr),
	mSocketDescriptor(mPORT, 0)
{
	//Not sure if GetMyBoundAddress() returns the external IP address - have to test that
	mpHost = std::make_shared<User>(sNextUniqueId++, hostUsername, AuthorityId::ADMIN, mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID()));
	mpConnectedUsers.insert({ mpHost->getUserId(), mpHost });

	updateServerUserInfo();
}

bool ChatRoomServer::startChatRoom()
{
	if (mIsRunning)
	{
		ChatRoomScene::printMessageToChatRoom("Server is already running.");
		return false;
	}
	else
	{
		mpPeer->Startup(mMAX_USERS, &mSocketDescriptor, 1);

		mpPeer->SetMaximumIncomingConnections(mMAX_USERS);

		ChatRoomScene::printMessageToChatRoom("Server is running!");
		mIsRunning = true;
	}

	return true;
}

void ChatRoomServer::update()
{
	receivePacket();
}

void ChatRoomServer::closeChatRoom()
{
	ServerClosingPacket serverClosing = ServerClosingPacket();

	//auto iter = mpConnectedUsers.begin();
	//for (; iter != mpConnectedUsers.end(); ++iter)
	//{
		broadcastPacket((const char*)(&serverClosing), sizeof(ServerClosingPacket));
	//}

	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
	ChatRoomServer::spInstance = nullptr;
	sNextUniqueId = 0;
}

std::shared_ptr<User> ChatRoomServer::getUserFromId(UserId userId)
{
	std::shared_ptr<User> foundUser = nullptr;

	std::map<UserId, std::shared_ptr<User>>::iterator iter = mpConnectedUsers.find(userId);
	if (iter != mpConnectedUsers.end())
	{
		foundUser = iter->second;		
	}

	return foundUser;
}

//Check if a packet is incoming
void ChatRoomServer::receivePacket()
{
	//Incoming packets to server from client.
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case PacketEventId::SET_AUTHORITY:
			break;

		case PacketEventId::SEND_PUBLIC_MESSAGE_REQUEST:
		{
			SendPublicMessageRequestPacket* data = (SendPublicMessageRequestPacket*)(mpPacket->data);

			SendPublicMessageRequestPacket messagePacket = SendPublicMessageRequestPacket(data->userId, data->message);

			std::shared_ptr<User> user = getUserFromId(data->userId);
			if (user != nullptr)
			{
				deliverPublicMessage(user, data->message);			
			}
			else
			{
				//Couldn't find user!!!
			}

			break;
		}
		case PacketEventId::SEND_PRIVATE_MESSAGE_REQUEST:
		{
			SendPrivateMessageRequestPacket* requestPacket = (SendPrivateMessageRequestPacket*)(mpPacket->data);

			UserId toUserId;

			auto iter = mpConnectedUsers.begin();
			for (; iter != mpConnectedUsers.end(); ++iter)
			{
				if (iter->second->getUsername().compare(requestPacket->toUsername) == 0)
				{
					toUserId = iter->second->getUserId();
					break;
				}
			}

			if (iter != mpConnectedUsers.end())
			{
				deliverPrivateMessage(requestPacket->fromUserId, toUserId, requestPacket->message);
			}

			break;
		}
		case PacketEventId::REQUEST_JOIN_SERVER:
		{
			RequestJoinServerPacket* requestPacket = (RequestJoinServerPacket*)(mpPacket->data);
			std::shared_ptr<User> newUser = addNewUser(*requestPacket, mpPacket->systemAddress);

			UserJoinedServerPacket userJoinedServerPacket = UserJoinedServerPacket(
				newUser->getUserId(),
				requestPacket->username
			);

			//Send to everyone in the server that a new user joined
			broadcastPacket((const char*)(&userJoinedServerPacket), sizeof(UserJoinedServerPacket));

			JoinAcceptedPacket joinAcceptedData = JoinAcceptedPacket(
				newUser->getUserId(),
				requestPacket->username,
				mMAX_USERS,
				(char)mpConnectedUsers.size(),
				mServerUserInfo
			);

			sendOncePacket((const char*)(&joinAcceptedData), sizeof(JoinAcceptedPacket), newUser->getAddress());

			//Print to server console
			std::string serverMessage = newUser->getUsername() + " has joined!";
			deliverPublicMessage(mpHost, serverMessage);

			break;
		}
		case PacketEventId::USER_JOINED_SERVER:
			ChatRoomScene::printMessageToChatRoom("User has joined the channel!");
			break;
		case PacketEventId::USER_LEFT_SERVER:
		{
			UserLeftServerPacket* userLeftPacket = (UserLeftServerPacket*)(mpPacket->data);
			std::shared_ptr<User> user = getUserFromId(userLeftPacket->userId);
			removeUser(userLeftPacket->userId);

			broadcastPacket((const char*)(userLeftPacket), sizeof(UserLeftServerPacket));

			ChatRoomScene::printMessageToChatRoom(user->getUsername() + " has left the server.");
			break;
		}
		case PacketEventId::SERVER_CLOSING:
			break;

		case PacketEventId::MUTE_USER:
			break;

		case PacketEventId::UNMUTE_USER:
			break;
		case ID_NEW_INCOMING_CONNECTION:
			//this just eats raknets trash
			break;
		default:
			ChatRoomScene::printMessageToChatRoom("I just got a packet, and I don't know how to read it!");
			break;
		}
	}
}

void ChatRoomServer::deliverPublicMessage(std::shared_ptr<User> user, std::string message)
{
	std::string decoratedMessage = User::formatMessage(user->getUsername(), message, user->getAuthority());

	DeliverPublicMessagePacket messagePacket = DeliverPublicMessagePacket(user->getUserId(), decoratedMessage);

	broadcastPacket((const char*)(&messagePacket), sizeof(DeliverPublicMessagePacket));

	ChatRoomScene::printMessageToChatRoom(decoratedMessage);
}

//Used for host only
void ChatRoomServer::deliverPersonalMessage(const std::string& userName, const std::string& message)
{
	UserId toUserId;

	auto iter = mpConnectedUsers.begin();
	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		if (iter->second->getUsername().compare(userName) == 0)
		{
			toUserId = iter->second->getUserId();
			break;
		}
	}

	if (iter != mpConnectedUsers.end())
	{
		deliverPrivateMessage(mpHost->getUserId(), toUserId, message);
	}

}

void ChatRoomServer::deliverPrivateMessage(UserId fromUserId, UserId toUserId, const std::string& message)
{
	std::shared_ptr<User> toUser = getUserFromId(toUserId);
	std::shared_ptr<User> fromUser = getUserFromId(fromUserId);

	std::string decoratedMessage = User::formatMessage(fromUser->getUsername() + " -> " + toUser->getUsername(), message, fromUser->getAuthority());

	DeliverPrivateMessagePacket privateMessagePacket = DeliverPrivateMessagePacket
	(
		fromUserId,
		toUserId,
		decoratedMessage
	);

	ChatRoomScene::printMessageToChatRoom(decoratedMessage);

	//Send to target user
	if (toUser != nullptr)
	{
		sendOncePacket((const char*)(&privateMessagePacket), sizeof(DeliverPrivateMessagePacket), toUser->getAddress());
	}

	//Send to original user
	//If original user is the host, we don't need to send it to them again
	if (fromUserId == mpHost.get()->getUserId()) return;

	if (fromUser != nullptr)
	{
		sendOncePacket((const char*)(&privateMessagePacket), sizeof(DeliverPrivateMessagePacket), fromUser->getAddress());
	}
}

//Call everytime a user joins or leaves the server
void ChatRoomServer::updateServerUserInfo()
{
	for (int userIndex = 0; userIndex < sMAX_USERS; ++userIndex)
	{
		// [id]["username"]
		// [*]["whatever"] (DELIMITER = '*') If '*' is in the starting index, then we can stop iterating through the array.
		mServerUserInfo[userIndex][0] = { DELIMITER };
	}

	int connecedUsers = (int)mpConnectedUsers.size();
	for (int userIndex = 0; userIndex < connecedUsers; ++userIndex)
	{
		for (int usernameIndex = 0; usernameIndex < sMAX_USERNAME_LENGTH; ++usernameIndex)
		{
			//We're at the id spot
			if (usernameIndex == 0)
			{
				mServerUserInfo[userIndex][0] = mpConnectedUsers.at(userIndex)->getUserId();
			}
			//We're at the start of the username
			else if (usernameIndex == 1)
			{
				std::shared_ptr<User> user = mpConnectedUsers.at(userIndex);
				memcpy(mServerUserInfo[userIndex], user->getUsername().c_str(), user->getUsername().length());
				continue;
			}
		}
	}
}

std::shared_ptr<User> ChatRoomServer::addNewUser(const RequestJoinServerPacket& requestPacket, RakNet::SystemAddress ipAddress)
{
	std::shared_ptr<User> newUser = std::make_shared<User>(
		sNextUniqueId++,
		requestPacket.username,
		AuthorityId::NORMAL,
		ipAddress
	);

	mpConnectedUsers.insert({ newUser->getUserId(), newUser });

	updateServerUserInfo();

	return newUser;
}

void ChatRoomServer::removeUser(UserId userId)
{
	auto iter = mpConnectedUsers.find(userId);

	if (iter == mpConnectedUsers.end())
	{
		ChatRoomScene::printMessageToChatRoom("Unable to remove user.");
	}
	else
	{
		mpConnectedUsers.erase(iter);
		updateServerUserInfo();
	}
}

void ChatRoomServer::broadcastPacket(const char* packetData, std::size_t packetSize)
{
	mpPeer->Send(packetData, (int)packetSize,
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ChatRoomServer::sendOncePacket(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress)
{
	mpPeer->Send(packetData, (int)packetSize,
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, ipAddress, false);
}

void ChatRoomServer::listUserInfoRequest()
{
	ChatRoomServer::spInstance->printUserInfo();
}

void ChatRoomServer::printUserInfo()
{
	auto iter = mpConnectedUsers.begin();

	ChatRoomScene::printMessageToChatRoom("");
	ChatRoomScene::printMessageToChatRoom("Active Users: " + std::to_string(mpConnectedUsers.size()) + "/" + std::to_string(mMAX_USERS));

	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		ChatRoomScene::printMessageToChatRoom("  " + std::to_string(iter->second->getUserId()) + " - " + iter->second->getUsername());
	}
}