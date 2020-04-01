#include <map>

#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>
#include <A3_DEMO/_andrick_Network/andrick_client.h>

UnitID BoidManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

BoidManager::BoidManager()
{
}

Boid* BoidManager::createUnit(const UserId& userId, bool shouldWrap, const SteeringData& steerData /*= ZERO_STEERING_DATA*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Boid* pUnit = NULL;

	//create unit
	pUnit = new Boid();//placement new

	UnitID theID = id;
	if (theID == INVALID_UNIT_ID)
	{
		theID = msNextUnitID;
		msNextUnitID++;
	}

	//assign id and increment nextID counter
	pUnit->mID = theID;

	//create some components
	pUnit->mpPosition = new BoidPosition(shouldWrap);
	pUnit->mpPosition->setData(posData);

	pUnit->mpPhysics = new BoidPhysics(pUnit->mpPosition);
	pUnit->mpPhysics->setData(physicsData);
	pUnit->mpPhysics->setPositionComponent(pUnit->mpPosition);

	pUnit->mpSteering = new BoidSteering();
	pUnit->mpSteering->setData(steerData);
	pUnit->mpSteering->setPhysicsComponent(pUnit->mpPhysics);

	//set max's
	pUnit->mMaxSpeed = MAX_SPEED;
	pUnit->mMaxAcc = MAX_ACC;
	pUnit->mMaxRotAcc = MAX_ROT_ACC;
	pUnit->mMaxRotVel = MAX_ROT_VEL;

	auto boidIter = mUnitMap.find(userId);
	BoidInfo* b = nullptr;
	Move m = { pUnit->getPhysicsComponent()->getData(), pUnit->getPositionComponent()->getData(), getTime() };

	if (boidIter != mUnitMap.end())
	{
		boidIter->second.insert({ theID, pUnit });
		b = mBoidInfoMap.find(userId)->second;
	}
	else
	{
		mUnitMap.insert({ userId, {{ theID, pUnit }} });
		mLerpCounter.insert({ userId, 0.0f });
		b = new BoidInfo();
		b->unprocessedMoves.insert({ theID, std::multiset<Move>() });
	}

	b->unprocessedMoves.find(theID)->second.insert(m);
	mBoidInfoMap.insert({ userId, b });

	return pUnit;
}

Boid* BoidManager::getPlayerUnit(const UserId& userId) const
{ 
	return getUnit(userId, PLAYER_UNIT_ID);
}

Boid* BoidManager::createPlayerUnit(const UserId& userId, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(userId, shouldWrap, ZERO_STEERING_DATA, posData, physicsData, PLAYER_UNIT_ID);
}

Boid* BoidManager::createRandomUnit(const UserId& userId)
{
	int posX = rand() % gDemoState->windowWidth;
	int posY =  rand() % gDemoState->windowHeight;
	//int velX = rand() % 50 - 25;
	//int velY = rand() % 40 - 20;

	a3vec2 tempPos;
	a3real2Set(tempPos.v, (a3real)posX, (a3real)posY);
	Boid* pUnit = createUnit(userId, true, ZERO_STEERING_DATA,PositionData(tempPos, 0));
	if (pUnit != NULL)
	{
		a3vec2 vec;
		a3real2Set(vec.v, (a3real)(gDemoState->windowWidth / 2), (a3real)(rand() % gDemoState->windowWidth / 2));
		pUnit->setSteering(userId, Steering::FLOCKING, vec);
		int temp = rand() % 360;
		temp = (int)(temp * (PI / 180));
		pUnit->getPositionComponent()->setFacing((float)temp);
	}

	return pUnit;
}

Boid* BoidManager::getUnit(const UserId& userId, const UnitID& unitId) const
{
	auto it = mUnitMap.find(userId);
	if (it != mUnitMap.end())//found client?
	{
		auto unitIter = it->second.find(unitId);
		if (unitIter != it->second.end())//found unit?
		{
			return unitIter->second;
		}
	}

	return nullptr;
}

void BoidManager::deleteUnit(const UserId& userId, const UnitID& unitId)
{
	auto it = mUnitMap.find(unitId);
	if (it != mUnitMap.end())//found client?
	{
		auto unitIter = it->second.find(unitId);
		if (unitIter != it->second.end())//found unit?
		{
			Boid* pUnit = unitIter->second;//hold for later
			BoidInfo* info = mBoidInfoMap.find(userId)->second;

			//remove from map
			mUnitMap.erase(it);
			//mBoidInfoMap.erase(it->first);
			mBoidInfoMap.erase(it->first);

			//remove components
			delete pUnit->mpPhysics;
			delete pUnit->mpPosition;
			delete pUnit->mpSteering;
			
			if (info)
				delete info;

			//call destructor
			pUnit->~Boid();

			

			//free the object in the pool
			//mPool.freeObject((Byte*)pUnit);
		}
	}
}

void BoidManager::deleteRandomUnit(const UserId& userId)
{
	if (mUnitMap.size() >= 1)
	{
		a3ui32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		a3ui32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(userId, it->first);
				break;
			}
		}
	}
}

void BoidManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		for (auto unitIter = it->second.begin(); unitIter != it->second.end(); ++unitIter)
		{
			unitIter->second->draw();
		}
	}
}

void BoidManager::updateAll(float elapsedTime)
{
	//Should we locally integrate a boid?

	std::map<UserId, BoidInfo*>::iterator& currentBoidInfoIter = mBoidInfoMap.begin();
	std::map<UserId, std::map<UnitID, Boid*>>::iterator& currentBoidsIter = mUnitMap.begin();
	for (; currentBoidInfoIter != mBoidInfoMap.end(); ++currentBoidsIter, ++currentBoidInfoIter)
	{
		//auto lerpCountIter = mLerpCounter.find(currentBoidInfoIter->first);
		//if (lerpCountIter != gDemoState->mpBoidManager->mLerpCounter.end())
		//{
		//	lerpCountIter->second += (float)gDemoState->renderTimer->secondsPerTick;
		//}

		//std::printf("UserID: %i | Current Iter:: %i \n", gDemoState->mpClient->getId(), currentBoidsIter->first);

		currentBoidInfoIter->second->timeSinceLastNetworkUpdate += (float)gDemoState->renderTimer->secondsPerTick;
		//std::printf("UserID: %i | Last Local Update IN MANAGER %f \n", gDemoState->mpClient->getId(), currentBoidInfoIter->second->timeSinceLastNetworkUpdate);

		//If we are the owning client, create the next move and simulate it
		if (currentBoidInfoIter->first == gDemoState->mpClient->getId())
		{
			simulateMove(currentBoidInfoIter->first, *currentBoidInfoIter->second);
		}
		//If we are a remote client, use hermite spline lerping and unprocessedMoves list to make smooth
		else
		{
			currentBoidInfoIter->second = createMove(currentBoidsIter->first);
				
			if (currentBoidInfoIter->second->timeBetweenNetworkUpdates < 0.0004f) return;
			
			float lerp = currentBoidInfoIter->second->timeSinceLastNetworkUpdate / currentBoidInfoIter->second->timeBetweenNetworkUpdates;

			std::printf("UserID: %i, Lerp: %f Last: %f Between: %f \n", 
				currentBoidInfoIter->first, lerp, currentBoidInfoIter->second->timeSinceLastNetworkUpdate, currentBoidInfoIter->second->timeBetweenNetworkUpdates);

			std::map<UnitID, Boid*>::iterator currentBoidIter = currentBoidsIter->second.begin();

			for (; currentBoidIter != currentBoidsIter->second.end(); ++currentBoidIter)
			{
				//If we have enough data, do lerp
				if (currentBoidInfoIter->second->unprocessedMoves.size() >= 2)
				{
					HermiteCubicSpline spline = createSpline(currentBoidInfoIter->second, currentBoidIter->second);

					a3vec2 newDerivative = spline.interpolateInterpDerivate(lerp);
					a3vec2 newVelocity = div(currentBoidInfoIter->second->timeBetweenNetworkUpdates, newDerivative);

					PhysicsData boidPhysicsData = currentBoidIter->second->getPhysicsComponent()->getData();
					boidPhysicsData.vel = newVelocity;

					PositionData boidPositionData = currentBoidIter->second->getPositionComponent()->getData();
					boidPositionData.pos = spline.targetLoc;

					//currentBoidIter->second->getPhysicsComponent()->setData(boidPhysicsData);
					//currentBoidIter->second->getPositionComponent()->setData(boidPositionData);
				}

				//Simulate
				//simulateMove(currentBoidInfoIter->first, *currentBoidInfoIter->second);
			}
		}
	}
}

BoidInfo* BoidManager::createMove(const UserId& userId)
{
	BoidInfo* boidMoveInfo = nullptr;

	std::map<UserId, BoidInfo*>::iterator userInfoIter = mBoidInfoMap.find(userId);
	std::map<UnitID, std::multiset<Move>>::iterator steeringIter = userInfoIter->second->unprocessedMoves.begin();

	for (; steeringIter != userInfoIter->second->unprocessedMoves.end(); ++steeringIter)
	{
		Move m = { getUnit(userId, steeringIter->first)->getPhysicsComponent()->getData(), getUnit(userId, steeringIter->first)->getPositionComponent()->getData(), getTime() };
		steeringIter->second.insert(m);
	}

	boidMoveInfo = userInfoIter->second;

	return boidMoveInfo;
}

void BoidManager::simulateMove(const UserId& userId, const BoidInfo& move)
{
	std::map<UnitID, std::multiset<Move>>::const_iterator steeringIter = move.unprocessedMoves.begin();

	for (; steeringIter != move.unprocessedMoves.end(); ++steeringIter)
	{
		Boid* currentUnit = getUnit(userId, steeringIter->first);
		
		if (currentUnit)
		{
			currentUnit->update((float)gDemoState->renderTimer->secondsPerTick);
		}
	}
}

HermiteCubicSpline BoidManager::createSpline(BoidInfo* boidInfo, Boid* currentBoid)
{
	HermiteCubicSpline spline;

	std::map<UnitID, std::multiset<Move>>::iterator unprocessedMoves = boidInfo->unprocessedMoves.begin();

	spline.startLoc = currentBoid->getPositionComponent()->getData().pos;
	spline.targetLoc = unprocessedMoves->second.rbegin()->positionData.pos;

	float velToDerivative = boidInfo->timeBetweenNetworkUpdates;
	spline.startDerivative = mult(velToDerivative, currentBoid->getPhysicsComponent()->getData().vel);
	spline.targetDerivative = mult(velToDerivative, unprocessedMoves->second.rbegin()->physicsData.vel);

	return spline;
}