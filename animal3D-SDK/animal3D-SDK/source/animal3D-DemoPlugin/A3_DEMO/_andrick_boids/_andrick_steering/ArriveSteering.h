#pragma once
#include <Trackable.h>
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

	Vector2D mTargetVelocity;
	float mTargetSpeed;
	float mStopRadius = 0.2;
	float mSlowRadius = 150;
	float mTimeToTarget = 0.1f;

};