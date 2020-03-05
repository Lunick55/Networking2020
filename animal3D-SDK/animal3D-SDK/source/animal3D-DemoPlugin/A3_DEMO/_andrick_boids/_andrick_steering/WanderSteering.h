#pragma once
#include "Steering.h"
#include "FaceSteering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	a3vec2 asVector(float num);

	virtual Steering* getSteering();

protected:
	float mWanderOffset = 100.0f;
	float mWanderRadius = 360.0f;
	float wanderRate = 1.0f;
	float mWanderOrientation;
	a3vec2 mTarget;

	FaceSteering mFaceSteering;
};