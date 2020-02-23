#pragma once

#include <Trackable.h>
#include "Steering.h"

class FaceSteering : public Steering
{
public:
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();
	Steering* getSteering(Vector2D diff);

	float mTargetAngle = 0.1;
	float mSlowAngle = 0.5;
	float mTimeToTarget = 0.1;

	float mTargetOrient;
	float mRotation;
	float mRotationSize;

	float mTargetRotation;
	float mAngularAcc;
	//float rotationDirection;
};