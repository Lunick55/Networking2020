#include <cassert>

#include "Steering.h"
#include "GroupAlignSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>


GroupAlignSteering::GroupAlignSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	a3vec2 diff;
	Boid* pOwner = NULL; //TODO: the manager reference //gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	mNeighborRadius = 1;//TODO: the manager reference // gpGame->getGroupAlignRadius();

	a3vec2 directionVector = a3vec2_zero;
	int neighborCount = 0;

	std::map<UnitID, Boid*> unitMap; //TODO: get map gpGame->getUnitManager()->getMap();
	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			a3vec2 unitPos = it->second->getPositionComponent()->getPosition();
			a3real2Diff(diff.v, unitPos.v, pOwner->getPositionComponent()->getPosition().v);
			if (a3real2Length(diff.v) < mNeighborRadius)
			{
				directionVector.x = directionVector.x + (float)cos(it->second->getFacing() - (PI / 2));
				directionVector.y = directionVector.y + (float)sin(it->second->getFacing() - (PI/2));

				neighborCount++;
			}
		}
	}
	
	if (neighborCount == 0)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		data.acc = a3vec2_zero;
		data.vel = a3vec2_zero;
		this->mData = data;
		return this;
	}
	a3real2DivS(directionVector.v, (float)neighborCount);
	a3real2Normalize(directionVector.v);

	mFaceSteering.getSteering(directionVector);

	data.acc = a3vec2_zero;
	data.rotAcc = mFaceSteering.getData().rotAcc;

	this->mData = data;
	return this;
}
