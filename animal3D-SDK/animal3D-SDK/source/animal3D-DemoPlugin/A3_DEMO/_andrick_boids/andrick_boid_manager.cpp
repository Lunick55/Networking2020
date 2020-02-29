#include <map>

#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_steering.h>

UnitID BoidManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

BoidManager::BoidManager()
{
}

Boid* BoidManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
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
	pUnit->mpSteering = new BoidSteering();
	pUnit->mpPhysics->setData(physicsData);

	//set max's
	pUnit->mMaxSpeed = MAX_SPEED;
	pUnit->mMaxAcc = MAX_ACC;
	pUnit->mMaxRotAcc = MAX_ROT_ACC;
	pUnit->mMaxRotVel = MAX_ROT_VEL;

	return pUnit;
}


Boid* BoidManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Boid* BoidManager::createRandomUnit(const Sprite& sprite)
{
	//TODO: figure out that damn SCREEN SIZE
	int posX = 0;//rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = 0;//rand() % gpGame->getGraphicsSystem()->getHeight();
	//int velX = rand() % 50 - 25;
	//int velY = rand() % 40 - 20;

	a3vec2 tempPos;
	a3real2Set(tempPos.v, (a3real)posX, (a3real)posY);
	Boid* pUnit = createUnit(sprite, true, PositionData(tempPos, 0));
	if (pUnit != NULL)
	{
		//TODO: figure out SCREEN SIZE
		//pUnit->setSteering(Steering::FLOCKING, a3vec2(gpGame->getGraphicsSystem()->getWidth() / 2, rand() % gpGame->getGraphicsSystem()->getHeight() / 2));
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
