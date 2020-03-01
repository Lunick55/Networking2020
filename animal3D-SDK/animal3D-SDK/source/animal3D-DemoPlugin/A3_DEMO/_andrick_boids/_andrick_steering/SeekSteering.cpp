#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>


SeekSteering::SeekSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeekSteering::getSteering()
{
	a3vec2 diff;
	Boid* pOwner = NULL; //TODO: gamekkmanager// = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Boid* pTarget = NULL;//TODO: mmmmanager gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		a3real2Diff(diff.v, mTargetLoc.v, pOwner->getPositionComponent()->getPosition().v);
	}
	else
	{
		a3real2Diff(diff.v, pOwner->getPositionComponent()->getPosition().v, mTargetLoc.v);
	}

	a3real2Normalize(diff.v);
	a3real2MulS(diff.v, pOwner->getMaxAcc());

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff;

	//data.rotVel = 1.0f;
	this->mData = data;
	return this;
}

