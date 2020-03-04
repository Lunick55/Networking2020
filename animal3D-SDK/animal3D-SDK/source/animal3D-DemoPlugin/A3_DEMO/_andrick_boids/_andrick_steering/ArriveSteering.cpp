//#include <cassert>

#include "Steering.h"
#include "ArriveSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>


ArriveSteering::ArriveSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveSteering::getSteering()
{
	//TODO: get boid manager for all the comments with gpGame

	a3vec2 diff;
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mOwnerID); 
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Boid* pTarget = gDemoState->mpBoidManager->getUnit(mTargetID);
		//assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	
	diff = mTargetLoc;// - pOwner->getPositionComponent()->getPosition();
	a3real2Sub(diff.v, pOwner->getPositionComponent()->getPosition().v);

	if (a3real2Length(diff.v) < mStopRadius)
	{
		data.acc = a3vec2_zero;
		data.vel = a3vec2_zero;
		this->mData = data;
		return this;
	}

	if (a3real2Length(diff.v) > mSlowRadius)
	{
		mTargetSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		mTargetSpeed = pOwner->getMaxSpeed() * a3real2Length(diff.v) / mSlowRadius;
	}
	
	mTargetVelocity = diff;
	a3real2Normalize(mTargetVelocity.v);
	a3real2MulS(mTargetVelocity.v, mTargetSpeed);

	data.acc = mTargetVelocity;// - data.vel;
	a3real2Sub(data.acc.v, data.vel.v);
	a3real2DivS(data.acc.v, mTimeToTarget);

	if (a3real2Length(diff.v) > pOwner->getMaxAcc())
	{
		a3real2Normalize(data.acc.v);
		a3real2MulS(data.acc.v, pOwner->getMaxAcc());
	}

	this->mData = data;
	return this;
}

