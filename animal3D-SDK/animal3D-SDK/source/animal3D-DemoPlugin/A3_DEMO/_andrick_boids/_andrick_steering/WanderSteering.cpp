#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>
#include <animal3D-A3DM/a3math/a3random.h>


WanderSteering::WanderSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	Boid* pOwner = NULL; //TODO: game manager gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	//update wander orientation
	mWanderOrientation += (a3random() * wanderRate);
	//calculate combined target Orient
	float targetOrientation;
	targetOrientation = mWanderOrientation + (pOwner->getFacing() - (PI / 2));

	a3vec2 product;
	a3real2ProductS(product.v, asVector(pOwner->getFacing() - (PI / 2)).v, mWanderOffset);
	//calculate center of wander cirlce
	a3real2Sum(mTarget.v, pOwner->getPositionComponent()->getPosition().v, product.v);

	//calculate target location
	a3real2ProductS(product.v, asVector(targetOrientation).v, mWanderRadius);
	a3real2Add(mTarget.v, product.v);

	//do a facing delegate to FACE using target
	mFaceSteering.setTargetLoc(mTarget);
	Steering* tempFace = mFaceSteering.getSteering();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.rotAcc = tempFace->getData().rotAcc;
	
	a3real2ProductS(data.acc.v, asVector(pOwner->getFacing() - (PI / 2)).v, pOwner->getMaxAcc());

	this->mData = data;
	return this;
}

a3vec2 WanderSteering::asVector(float num)
{
	a3vec2 temp;
	float x = (float)cos(num);
	float y = (float)sin(num);

	a3real2Set(temp.v, x, y);
	return temp;
}