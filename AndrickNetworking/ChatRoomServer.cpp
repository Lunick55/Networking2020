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
		std::cerr << "Server is already running." << std::endl;
		return false;
	}
	else
	{
		mpPeer->Startup(mMAX_USERS, &mSocketDescriptor, 1);

		mpPeer->SetMaximumIncomingConnections(mMAX_USERS);

		std::cout << "Server is running!" << std::endl;
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
	std::unique_ptr<ServerClosingPacket> serverClosing = std::make_unique<ServerClosingPacket>();

	auto iter = mpConnectedUsers.begin();
	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		//TODO: reimplement this
		//sendPacket(*serverClosing.get());

	}

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
			break;

		case PacketEventId::DELIVER_PUBLIC_MESSAGE:
			break;

		case PacketEventId::DELIVER_PRIVATE_MESSAGE:
			break;

		case PacketEventId::REQUEST_JOIN_SERVER:
		{
			RequestJoinServerPacket* requestPacket = (RequestJoinServerPacket*)(mpPacket->data);
			std::shared_ptr<User> newUser = addNewUser(*requestPacket, mpPacket->systemAddress);

			UserJoinedServerPacket userJoinedServerData = UserJoinedServerPacket(
				newUser->getUserId(),
				(char)mpConnectedUsers.size(),
				mServerUserInfo
			);

			JoinAcceptedPacket joinAcceptedData = JoinAcceptedPacket(
				newUser->getUserId(),
				requestPacket->username, 
				mMAX_USERS, 
				(char)mpConnectedUsers.size(),
				mServerUserInfo
			);

			//Send to client that just joined
			mpPeer->Send((const char*)(&joinAcceptedData), sizeof(JoinAcceptedPacket),
				PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
				0, newUser->getAddress(), false);

			//TODO: fix this? it doesn't seem to do anything, i think it's the UNASSIGNED_blahblahblah thing
			//Send to everyone else
			mpPeer->Send((const char*)(&userJoinedServerData), sizeof(UserJoinedServerPacket),
				PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
				0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			
			std::string serverMessage = newUser->getUsername() + " has joined!";
			deliverPublicMessage(newUser, serverMessage);

			break;
		}
		case PacketEventId::JOIN_ACCEPTED:
			break;

		case PacketEventId::USER_JOINED_SERVER:
			std::cout << "User has joined the channel!" << std::endl;
			break;

		case PacketEventId::USER_LEFT_SERVER:
			break;

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
			printf("I just got a packet, and I don't know how to read it!\n", mpPacket->data[0]);
			break;
		}
	}
}

void ChatRoomServer::deliverPublicMessage(std::shared_ptr<User> user, std::string message)
{
	std::string decoratedMessage = User::formatMessage(user->getUsername(), message, user->getAuthority());

	DeliverPublicMessagePacket messagePacket = DeliverPublicMessagePacket(user->getUserId(), decoratedMessage);

	mpPeer->Send((const char*)(&messagePacket), sizeof(DeliverPublicMessagePacket),
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	std::cout << decoratedMessage << std::endl;
}

void ChatRoomServer::deliverPrivateMessage()
{
//	const PrivateMessageCommandPacket& p = static_cast<const PrivateMessageCommandPacket&>(packet);
//
//	//Send to host
//	mpPeer->Send((const char*)(&packet), sizeof(PrivateMessageCommandPacket),
//		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
//		0, mpHost.get()->getAddress(), false);
//
//	//Send to target user
//	auto iter = mpConnectedUsers.find(p.userToSendTo);
//	if (iter != mpConnectedUsers.end())
//	{
//		const User& targetUser = *iter->second;
//
//		mpPeer->Send((const char*)(&packet), sizeof(PrivateMessageCommandPacket),
//			PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
//			0, targetUser.getAddress(), false);
//	}
//
//	//Send to original user
//	//If original user is the host, we don't need to send it to them again
//	if (p.userWhoExecutedCommand == mpHost.get()->getUserId()) return;
//
//	//We shouldn't need to check if they're in the list
//	//since this user just sent this command, but we can always 
//	//perform a check like above in the future if necessary
//	const User& targetUser = *mpConnectedUsers.find(p.userWhoExecutedCommand)->second;
//
//	mpPeer->Send((const char*)(&packet), sizeof(packet),
//		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
//		0, targetUser.getAddress(), false);
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
			if (usernameIndex == 0)
			{
				mServerUserInfo[userIndex][0] = mpConnectedUsers.at(userIndex)->getUserId();
			}
			else if (usernameIndex == 1)
			{
				std::shared_ptr<User> user = mpConnectedUsers.at(userIndex);
				memcpy(mServerUserInfo[userIndex], user->getUsername().c_str(), user->getUsername().length());
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
		std::cout << "Unable to remove user." << std::endl;
	}
	else
	{
		updateServerUserInfo();
	}
}