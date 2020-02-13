#include "andrick_host.h"
#include "andrick_user.h"
#include "../_andrick_Utils/_andrick_text_formatter.h"
#include "../_andrick_Scene/andrick_scene_tictactoe.h"
#include "../_andrick_Demostate/andrick_demostate.h"

std::shared_ptr<Host> Host::spInstance = nullptr;
UserId Host::sNextUniqueId = 0;

bool Host::isInitialized()
{
	return (spInstance != nullptr);
}

void Host::cleanup()
{
	if (Client::isHost())
	{
		Client::cleanup();
	}

	if (spInstance)
	{
		if (spInstance->mpPeer)
		{
			spInstance->closeServer();
		}

		if (spInstance->mpHost)
		{
			spInstance->mpHost.reset();
		}
	}

	spInstance.reset();
}

bool Host::initChatRoom(const int port, const int maxUsers, const std::string& hostUsername)
{
	if (!spInstance)
	{
		spInstance = std::make_shared<Host>(port, maxUsers, hostUsername);
	}

	return isInitialized();
}

Host::Host(const int port, const int maxUsers, const std::string& hostUsername) :
	mPORT(port), 
	mMAX_USERS(maxUsers), 
	mIsRunning(false),
	mpPeer(RakNet::RakPeerInterface::GetInstance()),
	mpPacket(nullptr),
	mSocketDescriptor(mPORT, 0)
{
	mpHost = std::make_shared<User>(sNextUniqueId++, hostUsername, AuthorityId::ADMIN, mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID()));
	Client::spInstance->mpClient = std::make_unique<User>(mpHost->getUserId(), hostUsername, AuthorityId::ADMIN, mpPeer->GetSystemAddressFromGuid(mpPeer->GetMyGUID()));
	mpConnectedUsers.insert({ mpHost->getUserId(), mpHost });

	updateServerUserInfo();
}

bool Host::startChatRoom(const a3_DemoState* demoState)
{
	if (mIsRunning)
	{
		demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Server is already running.", 2);
		return false;
	}
	else
	{
		mpPeer->Startup(mMAX_USERS, &mSocketDescriptor, 1);

		mpPeer->SetMaximumIncomingConnections(mMAX_USERS);

		demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Server is running!", 2);
		mIsRunning = true;
	}

	return true;
}


void Host::update(const a3_DemoState* demoState)
{
	receivePacket(demoState);
}

void Host::closeServer()
{
	ServerClosingPacket serverClosing = ServerClosingPacket();

	broadcastPacket((const char*)(&serverClosing), sizeof(ServerClosingPacket));

	mpPeer->Shutdown(1000);
	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
	sNextUniqueId = 0;
}

std::shared_ptr<User> Host::getUserFromId(UserId userId)
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
void Host::receivePacket(const a3_DemoState* demoState)
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
				deliverPublicMessage(demoState, user, data->message);
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
					if (iter != mpConnectedUsers.end())
					{
						deliverPrivateMessage(demoState, requestPacket->fromUserId, toUserId, requestPacket->message);
					}
					return;
				}
			}

			deliverPersonalMessage(demoState, mpConnectedUsers.find(requestPacket->fromUserId)->second->getUsername(), "Unknown user! Couldn't send your message.");

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
			deliverPublicMessage(demoState, mpHost, serverMessage);
			break;
		}
		case PacketEventId::USER_JOINED_SERVER:
		{
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "User has joined the channel!");
			break;
		}
		case PacketEventId::USER_LEFT_SERVER:
		{
			UserLeftServerPacket* userLeftPacket = (UserLeftServerPacket*)(mpPacket->data);
			std::shared_ptr<User> user = getUserFromId(userLeftPacket->userId);
			removeUser(demoState, userLeftPacket->userId);

			broadcastPacket((const char*)(userLeftPacket), sizeof(UserLeftServerPacket));

			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, user->getUsername() + " has left the server.");
			break;
		}
		case PacketEventId::SERVER_CLOSING:
			break;

		case PacketEventId::MUTE_USER:
			break;

		case PacketEventId::UNMUTE_USER:
			break;
		case PacketEventId::UPDATE_TICTAC_STATE:
		{
			UpdateTicTacState* updatedPacket = (UpdateTicTacState*)(Host::spInstance->mpPacket->data);

			for (int i = 0; i < GS_TICTACTOE_BOARD_HEIGHT; i++)
			{
				for (int j = 0; j < GS_TICTACTOE_BOARD_WIDTH; j++)
				{
					gs_tictactoe_setSpaceState(demoState->mpSceneManager->mpTictactoe->mTictacBoard, (gs_tictactoe_space_state)updatedPacket->tictactoeboard[i][j], i, j);
				}
			}

			Host::spInstance->broadcastPacket((const char*)(&Host::spInstance->mpPacket), sizeof(UpdateTicTacState));

			if (updatedPacket->fromUserId == demoState->mpSceneManager->mpTictactoe->mPlayer1Id && demoState->mpSceneManager->mpTictactoe->mPlayerType == TictactoeScene::PlayerType::PLAYER2)
			{
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "It's your turn!", 1, TextFormatter::GREEN);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::YOUR_TURN;
			}
			else if (updatedPacket->fromUserId == demoState->mpSceneManager->mpTictactoe->mPlayer2Id &&
				demoState->mpSceneManager->mpTictactoe->mPlayerType == TictactoeScene::PlayerType::PLAYER1)
			{
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Your turn has ended.", 1, TextFormatter::WHITE);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::OPPONENTS_TURN;
			}

			break;
		}
		case PacketEventId::SETUP_TICTAC_GAME:
		{
			SetupTictacGame* setupTictacPacket = (SetupTictacGame*)(Host::spInstance->mpPacket->data);

			demoState->mpSceneManager->mpTictactoe->mPlayer1Id = setupTictacPacket->player1Id;
			demoState->mpSceneManager->mpTictactoe->mPlayer2Id = setupTictacPacket->player2Id;
			demoState->mpSceneManager->mpTictactoe->mPlayer1Username = setupTictacPacket->player1Username;
			demoState->mpSceneManager->mpTictactoe->mPlayer2Username = setupTictacPacket->player2Username;

			if (Host::spInstance->mpHost->getUserId() == demoState->mpSceneManager->mpTictactoe->mPlayer1Id)
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::PLAYER1;
				//You are player 1!
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "You are player 1! Congrats! - X", 2, TextFormatter::GREEN);
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::WHITE);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::YOUR_TURN;
				demoState->mpSceneManager->mpTictactoe->mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_x;
			}
			else if (Host::spInstance->mpHost->getUserId() == demoState->mpSceneManager->mpTictactoe->mPlayer2Id)
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::PLAYER2;
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "You are player 2! Congrats! - O", 2, TextFormatter::GREEN);
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::WHITE);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::OPPONENTS_TURN;
				demoState->mpSceneManager->mpTictactoe->mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_o;
			}
			else
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::SPECTATOR;
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::SPECTOR, "You are a spectator :)", 2, TextFormatter::WHITE);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::SPECTATOR;
			}

			break;
		}
		case ID_NEW_INCOMING_CONNECTION:
			//this just eats raknets trash
			break;
		default:
		{
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "I just got a packet, and I don't know how to read it!");
			break;
		}
		}
	}
}

void Host::serverTravel(const a3_DemoState* demoState, SceneId id)
{
	if (Client::isHost())
	{
		ServerTravel serverTravelPacket = ServerTravel(id);
		Host::spInstance->broadcastPacket((const char*)(&serverTravelPacket), sizeof(ServerTravel));
		demoState->mpSceneManager->switchToScene(demoState, id);
	}
}

void Host::deliverPublicMessage(const a3_DemoState* demoState, std::shared_ptr<User> user, std::string message, MessageType type)
{
	std::string decoratedMessage = User::formatMessage(user->getUsername(), message, user->getAuthority());

	DeliverPublicMessagePacket messagePacket = DeliverPublicMessagePacket(user->getUserId(), decoratedMessage, type);

	broadcastPacket((const char*)(&messagePacket), sizeof(DeliverPublicMessagePacket));

	demoState->mpSceneManager->mpCurrentScene->addToChatList(type, decoratedMessage);
}

//Used for host only
void Host::deliverPersonalMessage(const a3_DemoState* demoState, const std::string& userName, const std::string& message)
{
	UserId toUserId;

	auto iter = mpConnectedUsers.begin();
	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		if (iter->second->getUsername().compare(userName) == 0)
		{
			toUserId = iter->second->getUserId();

			if (iter != mpConnectedUsers.end())
			{
				deliverPrivateMessage(demoState, mpHost->getUserId(), toUserId, message);
			}
			return;
		}
	}

	demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Unknown user! Couldn't send your message.");
}

void Host::deliverPrivateMessage(const a3_DemoState* demoState, UserId fromUserId, UserId toUserId, const std::string& message)
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

	demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, decoratedMessage);

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
void Host::updateServerUserInfo()
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

std::shared_ptr<User> Host::addNewUser(const RequestJoinServerPacket& requestPacket, RakNet::SystemAddress ipAddress)
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

void Host::removeUser(const a3_DemoState* demoState, UserId userId)
{
	auto iter = mpConnectedUsers.find(userId);

	if (iter == mpConnectedUsers.end())
	{
		demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Unable to remove user.");
	}
	else
	{
		mpConnectedUsers.erase(iter);
		updateServerUserInfo();
	}
}

void Host::broadcastPacket(const char* packetData, std::size_t packetSize)
{
	mpPeer->Send(packetData, (int)packetSize,
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Host::sendOncePacket(const char* packetData, std::size_t packetSize, RakNet::SystemAddress ipAddress)
{
	mpPeer->Send(packetData, (int)packetSize,
		PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
		0, ipAddress, false);
}

void Host::listUserInfoRequest(const a3_DemoState* demoState)
{
	Host::spInstance->printUserInfo(demoState);
}

void Host::printUserInfo(const a3_DemoState* demoState)
{
	auto iter = mpConnectedUsers.begin();

	demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "");
	demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Active Users: " + std::to_string(mpConnectedUsers.size()) + "/" + std::to_string(mMAX_USERS));

	for (; iter != mpConnectedUsers.end(); ++iter)
	{
		demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "  " + std::to_string(iter->second->getUserId()) + " - " + iter->second->getUsername());
	}
}