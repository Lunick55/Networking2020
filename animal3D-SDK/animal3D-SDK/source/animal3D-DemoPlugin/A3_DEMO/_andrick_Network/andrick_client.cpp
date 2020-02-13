#include "andrick_client.h"
#include "../_andrick_Utils/_andrick_text_formatter.h"
#include "../_andrick_Scene/andrick_scene_manager.h"
#include "../_andrick_Demostate/andrick_demostate.h"
#include "../_andrick_Scene/andrick_scene_tictactoe.h"
#include "../_andrick_Network/andrick_host.h"

std::shared_ptr<Client> Client::spInstance = nullptr;

bool Client::isInitialized()
{
	return (spInstance != nullptr);
}

void Client::cleanup()
{
	if (spInstance)
	{
		if (spInstance->mpPeer)
		{
			spInstance->leaveServer();
		}

		if (spInstance->mpClient)
		{
			spInstance->mpClient.reset();
		}
	}

	spInstance.reset();
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
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case PacketEventId::SERVER_TRAVEL:
		{
			ServerTravel* serverTravelPacket = (ServerTravel*)(Client::spInstance->mpPacket->data);
			demoState->mpSceneManager->switchToScene(demoState, serverTravelPacket->sceneId);
			break;
		}
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
		case PacketEventId::UPDATE_TICTAC_STATE:
		{
			UpdateTicTacState* updatedPacket = (UpdateTicTacState*)(Client::spInstance->mpPacket->data);

			for (int i = 0; i < GS_TICTACTOE_BOARD_HEIGHT; i++)
			{
				for (int j = 0; i < GS_TICTACTOE_BOARD_WIDTH; i++)
				{
					gs_tictactoe_setSpaceState(demoState->mpSceneManager->mpTictactoe->mTictacBoard, (gs_tictactoe_space_state)updatedPacket->tictactoeboard[i][j], i, j);
				}
			}

			if (Client::isHost())
			{
				Host::spInstance->broadcastPacket((const char*)(&Client::spInstance->mpPacket), sizeof(UpdateTicTacState));
			}

			if (updatedPacket->fromUserId == demoState->mpSceneManager->mpTictactoe->mPlayer1Id && demoState->mpSceneManager->mpTictactoe->mPlayerType == TictactoeScene::PlayerType::PLAYER2)
			{
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "It's your turn!", 1, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::YOUR_TURN;
			}
			else if (updatedPacket->fromUserId == demoState->mpSceneManager->mpTictactoe->mPlayer2Id && 
				demoState->mpSceneManager->mpTictactoe->mPlayerType == TictactoeScene::PlayerType::PLAYER1)
			{
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Your turn has ended.", 1, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::OPPONENTS_TURN;
			}
		}
		case PacketEventId::SETUP_TICTAC_GAME:
		{
			SetupTictacGame* setupTictacPacket = (SetupTictacGame*)(Client::spInstance->mpPacket->data);

			demoState->mpSceneManager->mpTictactoe->mPlayer1Id = setupTictacPacket->player1Id;
			demoState->mpSceneManager->mpTictactoe->mPlayer2Id = setupTictacPacket->player2Id;
			demoState->mpSceneManager->mpTictactoe->mPlayer1Username = setupTictacPacket->player1Username;
			demoState->mpSceneManager->mpTictactoe->mPlayer2Username = setupTictacPacket->player2Username;

			if (Client::spInstance->mpClient->getUserId() == demoState->mpSceneManager->mpTictactoe->mPlayer1Id)
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::PLAYER1;
				//You are player 1!
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "You are player 1! Congrats! - X", 2, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::YOUR_TURN;
				demoState->mpSceneManager->mpTictactoe->mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_x;
			}
			else if (Client::spInstance->mpClient->getUserId() == demoState->mpSceneManager->mpTictactoe->mPlayer2Id)
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::PLAYER2;
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "You are player 2! Congrats! - O", 2, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::PLAYER, "Type \"/play (1-9 on numpad)\" to pick your spot", 2, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::OPPONENTS_TURN;
				demoState->mpSceneManager->mpTictactoe->mPlayerSignature = gs_tictactoe_space_state::gs_tictactoe_space_o;
			}
			else
			{
				demoState->mpSceneManager->mpTictactoe->mPlayerType = TictactoeScene::PlayerType::SPECTATOR;
				demoState->mpSceneManager->mpTictactoe->addToChatList(MessageType::SPECTOR, "You are a spectator :)", 2, TextFormatter::BLACK);
				demoState->mpSceneManager->mpTictactoe->mCurrentStep = TictactoeScene::TicTacStep::SPECTATOR;
			}
		}
		default:
			demoState->mpSceneManager->mpCurrentScene->addToChatList(MessageType::EITHER, "Yo, ~I just got a packet, ~I just got a packet! ~I just got a packet, ~don't know; ~what it's; ~from!");
			break;
		}
	}
}

void Client::sendPublicMessage(const std::string& message, enum MessageType type)
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
	if (mpClient)
	{
		//Send a leaving packet.
		UserLeftServerPacket userLeavingPacket = UserLeftServerPacket(
			mpClient->getUserId()
		);

		mpPeer->Send((const char*)(&userLeavingPacket), sizeof(UserLeftServerPacket),
			PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED,
			0, mHostAddress, false);
	}

	mpPeer->Shutdown(500);
	RakNet::RakPeerInterface::DestroyInstance(mpPeer);
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
