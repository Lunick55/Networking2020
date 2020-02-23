#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::FACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (diff.getLength() == 0)
	{
		//Do nothing?
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}

	mTargetOrient = atan2(diff.getY(), diff.getX()) + (PI / 2);
	mRotation = mTargetOrient - pOwner->getFacing();
	mRotation = fmod(mRotation, (2 * PI));
	if (mRotation > PI)
	{
		mRotation -= PI;
		mRotation *= -1.0f;
	}
	if (mRotation < -PI)
	{
		mRotation += PI;
		mRotation *= -1.0f;
	}

	mRotationSize = abs(mRotation);

	mTargetRotation;

	if (mRotationSize < mTargetAngle)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	if (mRotationSize > mSlowAngle)
	{
		mTargetRotation = data.maxRotAcc;
	}
	else
	{
		mTargetRotation = data.maxRotAcc * mRotationSize / mSlowAngle;
	}

	mTargetRotation *= mRotation / mRotationSize;

	data.rotAcc = mTargetRotation - data.rotVel;
	data.rotAcc /= mTimeToTarget;

	float mAngularAcc = abs(data.rotAcc);

	if (mAngularAcc > data.maxRotAcc)
	{
		data.rotAcc /= mAngularAcc;
		data.rotAcc *= data.maxRotAcc;
	}

	data.acc = 0;
	this->mData = data;
	
	return this;
}

Steering* FaceSteering::getSteering(Vector2D diff)
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//if (diff.getLength() == 0)
	//{
	//	//Do nothing?
	//	data.rotAcc = 0;
	//	data.rotVel = 0;
	//	this->mData = data;
	//	return this;
	//}

	mTargetOrient = atan2(diff.getY(), diff.getX()) + (PI / 2);
	mRotation = mTargetOrient - (pOwner->getFacing());
	mRotation = fmod(mRotation, (2 * PI));
	if (mRotation > PI)
	{
		//mRotation -= PI;
		mRotation -= (2*PI);
		//mRotation *= -1.0f;
	}
	if (mRotation < -PI)
	{
		mRotation += (2*PI);
		//mRotation *= -1.0f;
	}

	mRotationSize = abs(mRotation);

	if (mRotationSize < mTargetAngle)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	if (mRotationSize > mSlowAngle)
	{
		mTargetRotation = data.maxRotAcc;
	}
	else
	{
		mTargetRotation = data.maxRotAcc * mRotationSize / mSlowAngle;
	}

	mTargetRotation *= mRotation / mRotationSize;

	data.rotAcc = mTargetRotation - data.rotVel;
	data.rotAcc /= mTimeToTarget;

	float mAngularAcc = abs(data.rotAcc);

	if (mAngularAcc > data.maxRotAcc)
	{
		data.rotAcc /= mAngularAcc;
		data.rotAcc *= data.maxRotAcc;
	}

	data.acc = 0;
	this->mData = data;

	return this;
}