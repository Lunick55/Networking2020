#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>


FaceSteering::FaceSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	a3vec2 diff;
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	//diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	a3real2Diff(diff.v, mTargetLoc.v, pOwner->getPositionComponent()->getPosition().v);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (a3real2Length(diff.v) == 0)
	{
		//Do nothing?
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}

	mTargetOrient = (float)atan2(diff.x, diff.y) + (PI / 2);
	mRotation = mTargetOrient - pOwner->getFacing();
	mRotation = (float)fmod(mRotation, (2 * PI));
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

	data.acc = a3vec2_zero;
	this->mData = data;
	
	return this;
}

Steering* FaceSteering::getSteering(a3vec2 diff)
{
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mOwnerID);
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

	mTargetOrient = (float)atan2(diff.y, diff.x) + (PI / 2);
	mRotation = mTargetOrient - (pOwner->getFacing());
	mRotation = (float)fmod(mRotation, (2 * PI));
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

	data.acc = a3vec2_zero;
	this->mData = data;

	return this;
}