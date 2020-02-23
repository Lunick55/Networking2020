#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	Vector2D asVector(float num);

	virtual Steering* getSteering();

protected:
	float mWanderOffset = 100.0f;
	float mWanderRadius = 30.0f;
	float wanderRate = 1.0;
	float mWanderOrientation;
	Vector2D mTarget;

	FaceSteering mFaceSteering;
};