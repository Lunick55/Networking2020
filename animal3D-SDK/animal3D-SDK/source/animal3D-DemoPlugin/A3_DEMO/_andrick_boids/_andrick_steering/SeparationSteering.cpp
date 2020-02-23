#include <cassert>

#include "Steering.h"
#include "SeparationSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeparationSteering::SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mSeekSteering(SeekSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEPARATION;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeparationSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D positionVector;
	int neighborCount = 0;
	mNeighborRadius = gpGame->getSeparationRadius();

	std::map<UnitID, Unit*> unitMap = gpGame->getUnitManager()->getMap();

	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			Vector2D unitPos = it->second->getPositionComponent()->getPosition();
			diff = unitPos - pOwner->getPositionComponent()->getPosition();
			if (diff.getLength() < mNeighborRadius)
			{
				positionVector += it->second->getPositionComponent()->getPosition();
				neighborCount++;
			}
		}
	}

	if (neighborCount == 0)
	{
		//set the values
		data.rotAcc = 0;
		data.rotVel = 0;
		data.acc = 0;
		data.vel = 0;
		this->mData = data;
		return this;
	}
	positionVector /= neighborCount;


	mSeekSteering.setTargetLoc(positionVector);
	mSeekSteering.getSteering();

	data.rotAcc = 0;
	data.rotVel = 0;
	data.acc = mSeekSteering.getData().acc * -1;
	
	this->mData = data;
	return this;
}

