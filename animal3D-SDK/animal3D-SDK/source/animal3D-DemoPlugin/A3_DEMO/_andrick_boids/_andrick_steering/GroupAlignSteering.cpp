#include <cassert>

#include "Steering.h"
#include "GroupAlignSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>


GroupAlignSteering::GroupAlignSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(userID), mFaceSteering(FaceSteering(userID, ownerID, targetLoc, targetID, shouldFlee))
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
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mUserID, mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	mNeighborRadius = gDemoState->mGroupAlignRadius;

	a3vec2 directionVector = a3vec2_zero;
	int neighborCount = 0;

	std::map<UserId, std::map<UnitID, Boid*>> unitMap = gDemoState->mpBoidManager->getMap();
	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		for (auto unitIter = it->second.begin(); unitIter != it->second.end(); ++unitIter)
		{
			if (unitIter->second != pOwner)
			{
				a3vec2 unitPos = unitIter->second->getPositionComponent()->getPosition();
				a3real2Diff(diff.v, unitPos.v, pOwner->getPositionComponent()->getPosition().v);
				if (a3real2Length(diff.v) < mNeighborRadius)
				{
					directionVector.x = directionVector.x + (float)cos(unitIter->second->getFacing() - (PI / 2));
					directionVector.y = directionVector.y + (float)sin(unitIter->second->getFacing() - (PI / 2));

					neighborCount++;
				}
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
