#include <cassert>

#include "Steering.h"
#include "GroupAlignSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


GroupAlignSteering::GroupAlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::GROUPALIGN;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* GroupAlignSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	mNeighborRadius = gpGame->getGroupAlignRadius();

	Vector2D directionVector = Vector2D(0,0);
	int neighborCount = 0;

	std::map<UnitID, Unit*> unitMap = gpGame->getUnitManager()->getMap();
	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			Vector2D unitPos = it->second->getPositionComponent()->getPosition();
			diff = unitPos - pOwner->getPositionComponent()->getPosition();
			if (diff.getLength() < mNeighborRadius)
			{
				directionVector.setX(directionVector.getX() + cos(it->second->getFacing() - (PI/2)));// getPhysicsComponent()->getData().vel;
				directionVector.setY(directionVector.getY() + sin(it->second->getFacing() - (PI/2)));// getPhysicsComponent()->getData().vel;

				neighborCount++;
			}
		}
	}
	
	if (neighborCount == 0)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		data.acc = 0;
		data.vel = 0;
		this->mData = data;
		return this;
	}
	directionVector /= neighborCount;	
	directionVector.normalize();

	mFaceSteering.getSteering(directionVector);

	data.acc = 0;
	data.rotAcc = mFaceSteering.getData().rotAcc;

	this->mData = data;
	return this;
}
