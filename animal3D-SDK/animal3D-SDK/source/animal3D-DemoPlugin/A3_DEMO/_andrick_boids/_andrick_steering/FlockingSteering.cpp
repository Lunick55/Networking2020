#include <cassert>

#include "Steering.h"
#include "FlockingSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


FlockingSteering::FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mWeightCohesion = gpGame->getCohesionWeight();//0.8f;
	mWeightSeparation = gpGame->getSeparationWeight();//1.0f;
	mWeightGroupAlign = gpGame->getGroupAlignWeight();//1.0f;

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	mCohesion.getSteering();
	mSeparation.getSteering();
	mGroupAlign.getSteering();
	mWander.getSteering();

	data.acc = (mCohesion.getData().acc * mWeightCohesion) + (mSeparation.getData().acc * mWeightSeparation) + (mGroupAlign.getData().acc * mWeightGroupAlign) + (mWander.getData().acc);
	data.rotAcc = (mCohesion.getData().rotAcc * mWeightCohesion) + (mSeparation.getData().rotAcc * mWeightSeparation) + (mGroupAlign.getData().rotAcc * mWeightGroupAlign) + (mWander.getData().rotAcc);

	this->mData = data;
	return this;
}

