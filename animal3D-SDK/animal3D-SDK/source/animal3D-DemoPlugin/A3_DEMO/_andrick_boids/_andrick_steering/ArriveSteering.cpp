#include <cassert>

#include "Steering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (diff.getLength() < mStopRadius)
	{
		data.acc = 0;
		data.vel= 0;
		this->mData = data;
		return this;
	}

	if (diff.getLength() > mSlowRadius)
	{
		mTargetSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		mTargetSpeed = pOwner->getMaxSpeed() * diff.getLength() / mSlowRadius;
	}
	
	mTargetVelocity = diff;
	mTargetVelocity.normalize();
	mTargetVelocity *= mTargetSpeed;

	data.acc = mTargetVelocity - data.vel; //pOwner->getPhysicsComponent()->getVelocity();
	data.acc /= mTimeToTarget;

	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	this->mData = data;
	return this;
}

