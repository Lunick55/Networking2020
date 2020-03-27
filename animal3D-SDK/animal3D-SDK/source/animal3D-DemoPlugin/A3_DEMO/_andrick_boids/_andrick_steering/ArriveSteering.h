#pragma once
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UserId& userId, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

	a3vec2 mTargetVelocity;
	float mTargetSpeed;
	float mStopRadius = 0.2f;
	float mSlowRadius = 150.0f;
	float mTimeToTarget = 0.1f;

};