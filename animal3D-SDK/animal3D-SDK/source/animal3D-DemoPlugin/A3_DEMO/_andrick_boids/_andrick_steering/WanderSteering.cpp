#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: mWanderOrientation(0), Steering(), mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
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

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	//update wander orientation
	mWanderOrientation += (genRandomBinomial() * wanderRate);

	//calculate combined target Orient
	float targetOrientation;
	targetOrientation = mWanderOrientation + (pOwner->getFacing() - (PI / 2));

	//calculate center of wander cirlce
	mTarget = pOwner->getPositionComponent()->getPosition() + (asVector(pOwner->getFacing() - (PI / 2)) * mWanderOffset);

	//calculate target location
	mTarget += (asVector(targetOrientation) * mWanderRadius);

	//do a facing delegate to FACE using target
	mFaceSteering.setTargetLoc(mTarget);
	Steering* tempFace = mFaceSteering.getSteering();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.rotAcc = tempFace->getData().rotAcc;
	
	data.acc = asVector(pOwner->getFacing() - (PI / 2)) * pOwner->getMaxAcc();

	this->mData = data;
	return this;
}

Vector2D WanderSteering::asVector(float num)
{
	Vector2D temp;
	float x = cos(num);
	float y = sin(num);

	temp.setX(x);
	temp.setY(y);

	return temp;
}