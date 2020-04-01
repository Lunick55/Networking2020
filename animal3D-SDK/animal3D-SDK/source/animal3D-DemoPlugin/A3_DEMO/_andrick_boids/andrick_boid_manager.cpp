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

	//place unit in map
	auto boidIter = mUnitMap.find(userId);

	if (boidIter != mUnitMap.end())
	{
		boidIter->second.insert({ theID, pUnit });
	}
	else
	{
		auto newUserMap = mUnitMap.insert({ userId, {{ theID, pUnit }} }).first;
		//newUserMap->second.insert({ theID, pUnit });
	}

	//place info in map
	auto boidInfoIter = mBoidInfoMap.find(userId);

	if (boidInfoIter != mBoidInfoMap.end())
	{
		boidInfoIter->second = BoidInfo();
	}
	else
	{
		mBoidInfoMap.insert({ userId, BoidInfo() });
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

			//remove from map
			mUnitMap.erase(it);

			//remove components
			delete pUnit->mpPhysics;
			delete pUnit->mpPosition;
			delete pUnit->mpSteering;

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

	std::map<UserId, BoidInfo>::iterator boidInfo = mBoidInfoMap.begin();
	std::map<UserId, std::map<UnitID, Boid*>>::iterator it = mUnitMap.begin();
	for (; it != mUnitMap.end(); ++it, ++boidInfo)
	{
		//std::printf("UserID: %i\n", gDemoState->mpClient->getId());

		if (it->first == gDemoState->mpClient->getId())
		{
			for (auto unitIter = it->second.begin(); unitIter != it->second.end(); ++unitIter)
			{
				unitIter->second->update(elapsedTime);
			}

			//boidInfo->second.timeSinceLastLocalUpdate += elapsedTime;
			//
			////Make sure this isn't 0
			//if (boidInfo->second.timeBetweenLastNetworkUpdate < 0.0004f) return;
			//
			//a3f64 lerpRatio = boidInfo->second.timeSinceLastLocalUpdate / boidInfo->second.timeBetweenLastNetworkUpdate;
			//
			//std::printf("UserID: %i, Lerp: %f", it->first, lerpRatio);
			////Do lerp stuff here
			////Lerp position
			////Lerp velocity
			////Lerp acceleration
		}	//
	}
}
