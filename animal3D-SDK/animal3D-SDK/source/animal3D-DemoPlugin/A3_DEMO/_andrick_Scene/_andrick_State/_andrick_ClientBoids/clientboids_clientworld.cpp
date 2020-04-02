#include <A3_DEMO/_andrick_Scene/_andrick_State/_andrick_ClientBoids/clientboids_clientworld.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_menuoption.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_clientboids.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>
#include <A3_DEMO/_andrick_Network/andrick_server.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>
#include <A3_DEMO/_andrick_Network/_andrick_Packet/andrick_packethandler.h>
#include <A3_DEMO/_andrick_Scene/andrick_scene_mainmenu.h>
#include <A3_DEMO/_andrick_Scene/_andrick_Input/andrick_sceneinputhandler.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

ClientBoidsClientWorld::ClientBoidsClientWorld(std::shared_ptr<Scene> parentScene) :
	SceneState(parentScene, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CLIENT_WORLD, DARK_GREY),
	mChatHistory(10),
	mDataMode(andrick_ID_BOID_DATA_PUSH_EVENT),
	mDataModeText("Data Push")
{
	setEscapeOption(MenuOption(a3key_escape, "<-- [ESC to disconnect]", nullptr, SceneId::CLIENT_BOIDS, (SceneStateId)ClientBoidsScene::ClientBoidsStateId::CONFIRM_DISCONNECT));
}

void ClientBoidsClientWorld::enteringState()
{
	Color boidColor = AndrickColors::createColor(
		rand() % 255 / 255.0f + 0.2f,
		rand() % 255 / 255.0f + 0.2f,
		rand() % 255 / 255.0f + 0.2f,
		1.0f);

	printf("%f %f %f", boidColor.r, boidColor.g, boidColor.b);

	for (int i = 0; i < BOID_COUNT; i++)
	{
		Boid* pBoid = gDemoState->mpBoidManager->createRandomUnit(gDemoState->mpClient->getId());
		pBoid->setColor(boidColor);

		if (!pBoid)
		{
			gDemoState->mpBoidManager->deleteRandomUnit(gDemoState->mpClient->getId());
		}
	}

	SceneState::enteringState();
}

void ClientBoidsClientWorld::processInput()
{
	SceneState::processInput();
	mpInputHandler->clearCurrentInput();
}

void ClientBoidsClientWorld::processIncomingEvent(std::shared_ptr<Event> evnt)
{
	switch (evnt->eventId)
	{
	case EventId::BOID_DATA_EVENT:
	{
		std::shared_ptr<BoidDataEvent> boidEvnt = std::dynamic_pointer_cast<BoidDataEvent>(evnt);
		handleBoidDataEvents(boidEvnt);
		break;
	}
	case EventId::USER_DISCONNECTED:
	{
		std::shared_ptr<UserDisconnectedEvent> disconnectEvnt = std::dynamic_pointer_cast<UserDisconnectedEvent>(evnt);
		removeBoidData(disconnectEvnt);
		break;
	}
	default:
		break;
	}
}

void ClientBoidsClientWorld::update()
{
	//We need to have a local copy of all the online boids.
	//Store client boid data in client.
	//We have a map of all the connected clients
	//Populate this when a new client joins

	//We get boid updates each frame
	//Get client by id from each boid update
	//Update the client boid vel, acc, and pos, with the new boid data

	//Iterate through all the clients
	//Integrate using the vel, acc, and pos data.

	gDemoState->mpBoidManager->updateAll((float)gDemoState->renderTimer->secondsPerTick);
}

void ClientBoidsClientWorld::queueOutgoingEvents()
{
	BoidData localBoidData[BOID_COUNT];
	
	UserId userId = gDemoState->mpClient->getId();
	Color boidColor = gDemoState->mpBoidManager->getUnit(userId, 1)->getColor();
	for (int i = 0; i < BOID_COUNT; i++)
	{
		localBoidData[i].boidPositionData = gDemoState->mpBoidManager->getUnit(userId, i + 1)->getPositionComponent()->getData();
		localBoidData[i].boidPhysicsData = gDemoState->mpBoidManager->getUnit(userId, i + 1)->getPhysicsComponent()->getData();
		localBoidData[i].boidID = i + 1;
	}

	std::shared_ptr<BoidDataEvent> packetData = std::make_shared<BoidDataEvent>(andrick_ID_BOID_DATA_PUSH_EVENT, getTime(), boidColor, localBoidData, userId);
	gEventSystem.queueNetworkEvent(packetData);
}

void ClientBoidsClientWorld::render()
{
	SceneState::render();
	renderEscapeOption();

	gTextFormatter.setLine(1);
	gTextFormatter.drawText("Welcome to the Boids Server!", WHITE, TextAlign::CENTER_X);

	gTextFormatter.setLine(5);
	renderMenuOptions(WHITE, TextAlign::LEFT);

	//TODO: Lerp between incoming and previous
	gDemoState->mpBoidManager->drawAll();

	gTextFormatter.setLine(4);
	gTextFormatter.drawText(
		std::to_string(gDemoState->mpClient->getConnectedUserCount()) + "/" +
		std::to_string(gDemoState->mpClient->getMaxUserCount()) + " Clients Online",
		WHITE, TextAlign::RIGHT
	);

	gTextFormatter.offsetLine(2);
	renderChatLogHistory(mChatLogHistory, TextAlign::RIGHT, 1);

	//gTextFormatter.drawText(std::to_string(gDemoState->mpClient->getId()));
}

void ClientBoidsClientWorld::exitingState()
{
	SceneState::exitingState();

	for (std::size_t userID = 0; userID < gDemoState->mpClient->getConnectedUserCount(); ++userID)
	{
		for (int unitID = 0; unitID < BOID_COUNT; ++unitID)
		{
			gDemoState->mpBoidManager->deleteRandomUnit((UserId)userID);
		}
	}

	mDataMode = andrick_ID_BOID_DATA_PUSH_EVENT;
	mDataModeText = "Data Push";
}

void ClientBoidsClientWorld::handleBoidDataEvents(std::shared_ptr<BoidDataEvent> boidEvnt)
{
	//printf("Getting push...");

	//Don't update our boids with the packet we sent to the server.
	if (boidEvnt->userId == gDemoState->mpClient->getId())
		return;

	if (!gDemoState->mpBoidManager->isUserRegistered(boidEvnt->userId))
	{
		for (int i = 0; i < BOID_COUNT; i++)
		{
			Boid* pBoid = gDemoState->mpBoidManager->createUnit(boidEvnt->userId, true, ZERO_STEERING_DATA,
				boidEvnt->boids[i].boidPositionData, boidEvnt->boids[i].boidPhysicsData, boidEvnt->boids[i].boidID);

			pBoid->setColor(boidEvnt->boidColor);
		}
	}
	else
	{
		//auto lerpCountIter = gDemoState->mpBoidManager->mLerpCounter.find(boidEvnt->userId);
		//
		//if (lerpCountIter != gDemoState->mpBoidManager->mLerpCounter.end())
		//{
		//	lerpCountIter->second = 0.0f;
		//}

		for (int i = 0; i < BOID_COUNT; i++)
		{
			const BoidData& currentBoid = boidEvnt->boids[i];
			const Boid* boid = gDemoState->mpBoidManager->getUnit(boidEvnt->userId, currentBoid.boidID);

			if (boid)
			{
				//On Update
					//Iterate through all of the userinfo and add deltatime to time since last local update
					//If local client, update boid normally
					//If remote client, attempt to do hermite spline smoothing using userinfo physics/position data and set velocity, position based on that

				//On New Packet
					//Get userinfo for userid and set time between last network update = to time since last local update
					//Set time since last local update to zero
					//In userinfo, add new physics/position data and remove physics/position data that is more than 3 old

				BoidInfo* info = gDemoState->mpBoidManager->getBoidInfo(boidEvnt->userId);

				if (info)
				{
					info->timeBetweenNetworkUpdates = info->timeSinceLastNetworkUpdate;

					//It doesn't like this for some reason?
					//info->timeSinceLastNetworkUpdate = 0.0f;
					//

					std::printf("Last Local Update IN WORLD %f \n", info->timeSinceLastNetworkUpdate);

					Move m = { currentBoid.boidPhysicsData, currentBoid.boidPositionData, boidEvnt->timestamp };

					std::map<UnitID, std::multiset<Move>>::iterator& allUnitMovesIter = info->unprocessedMoves.find(currentBoid.boidID);

					if (allUnitMovesIter == info->unprocessedMoves.end())
					{
						allUnitMovesIter = info->unprocessedMoves.insert({ currentBoid.boidID, std::multiset<Move>() }).first;
					}

					std::multiset<Move> currentKnownUnitMoves = allUnitMovesIter->second;
					currentKnownUnitMoves.insert(m);

					std::multiset<Move> updatedMoves;

					Move mostRecentMove = *currentKnownUnitMoves.rbegin();

					//Timestamp always ordered oldest to newest (multiset sorts by least to greatest).
					for (auto currentKnownIter = currentKnownUnitMoves.rbegin(); currentKnownIter != currentKnownUnitMoves.rend(); ++currentKnownIter)
					{
						//if (currentKnownIter->timeStamp >= mostRecentMove.timeStamp)
						//{
							//mostRecentMove = *currentKnownIter;
							updatedMoves.insert(mostRecentMove);
						//}
					
						if (updatedMoves.size() == 2)
						{
							break;
						}
					}

					allUnitMovesIter->second = updatedMoves;

					//Shouldnt be here ideally for lerping but idk how to get lerping to work
					boid->getPositionComponent()->setData(currentBoid.boidPositionData);
					boid->getPhysicsComponent()->setData(currentBoid.boidPhysicsData);
				}
			}
		}
	}
}

void ClientBoidsClientWorld::removeBoidData(std::shared_ptr<UserDisconnectedEvent> disconnectEvnt)
{
	if (gDemoState->mpBoidManager->isUserRegistered(disconnectEvnt->userId))
	{
		for (int unitID = 0; unitID < BOID_COUNT; ++unitID)
		{
			gDemoState->mpBoidManager->deleteRandomUnit(disconnectEvnt->userId);
		}
	}
}