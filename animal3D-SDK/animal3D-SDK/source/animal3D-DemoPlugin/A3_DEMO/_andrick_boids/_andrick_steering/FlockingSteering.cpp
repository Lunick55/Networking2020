//#include <cassert>

#include "Steering.h"
#include "FlockingSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

FlockingSteering::FlockingSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mCohesion(CohesionSteering(ownerID, targetLoc, targetID, shouldFlee)), mSeparation(SeparationSteering(ownerID, targetLoc, targetID, shouldFlee)), mGroupAlign(GroupAlignSteering(ownerID, targetLoc, targetID, shouldFlee)), mWander(WanderSteering(ownerID, targetLoc, targetID, shouldFlee))
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

Steering* FlockingSteering::getSteering()
{
	//TODO: get the stuff from gpGame
	//a3vec2 diff;
	Boid* pOwner = NULL;// gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mWeightCohesion = 0.8f;//gpGame->getCohesionWeight();//0.8f;
	mWeightSeparation = 1.0f;//gpGame->getSeparationWeight();//1.0f;
	mWeightGroupAlign = 1.0f;//gpGame->getGroupAlignWeight();//1.0f;

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Boid* pTarget = NULL;// gpGame->getUnitManager()->getUnit(mTargetID);
		//assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	mCohesion.getSteering();
	mSeparation.getSteering();
	mGroupAlign.getSteering();
	mWander.getSteering();

	//finish making dans fucking vectors work
	//UNDONE: data.acc = (mCohesion.getData().acc * mWeightCohesion) + (mSeparation.getData().acc * mWeightSeparation) + (mGroupAlign.getData().acc * mWeightGroupAlign) + (mWander.getData().acc);
	data.rotAcc = (mCohesion.getData().rotAcc * mWeightCohesion) + (mSeparation.getData().rotAcc * mWeightSeparation) + (mGroupAlign.getData().rotAcc * mWeightGroupAlign) + (mWander.getData().rotAcc);

	this->mData = data;
	return this;
}

