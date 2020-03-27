#pragma once
#include "Steering.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "GroupAlignSteering.h"
#include "WanderSteering.h"

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	CohesionSteering mCohesion;
	SeparationSteering mSeparation;
	GroupAlignSteering mGroupAlign;
	WanderSteering mWander;

	float mWeightCohesion;
	float mWeightSeparation;
	float mWeightGroupAlign;
};