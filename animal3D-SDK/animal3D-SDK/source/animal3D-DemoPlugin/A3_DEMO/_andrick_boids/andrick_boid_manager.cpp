#include <map>

#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

UnitID BoidManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

BoidManager::BoidManager()
{
}

Boid* BoidManager::createUnit(bool shouldWrap, const SteeringData& steerData /*= ZERO_STEERING_DATA*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
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

	//place in map
	mUnitMap[theID] = pUnit;

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


Boid* BoidManager::createPlayerUnit(bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(shouldWrap, ZERO_STEERING_DATA, posData, physicsData, PLAYER_UNIT_ID);
}


Boid* BoidManager::createRandomUnit()
{
	int posX = rand() % gDemoState->windowWidth;
	int posY =  rand() % gDemoState->windowHeight;
	//int velX = rand() % 50 - 25;
	//int velY = rand() % 40 - 20;

	a3vec2 tempPos;
	a3real2Set(tempPos.v, (a3real)posX, (a3real)posY);
	Boid* pUnit = createUnit(true, ZERO_STEERING_DATA,PositionData(tempPos, 0));
	if (pUnit != NULL)
	{
		a3vec2 vec;
		a3real2Set(vec.v, (a3real)(gDemoState->windowWidth / 2), (a3real)(rand() % gDemoState->windowWidth / 2));
		pUnit->setSteering(Steering::FLOCKING, vec);
		int temp = rand() % 360;
		temp = (int)(temp * (PI / 180));
		pUnit->getPositionComponent()->setFacing((float)temp);
	}

	return pUnit;
}

Boid* BoidManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void BoidManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Boid* pUnit = it->second;//hold for later

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

void BoidManager::deleteRandomUnit()
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
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void BoidManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void BoidManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}
