#pragma once
#include "Steering.h"

class FaceSteering : public Steering
{
public:
	FaceSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();
	Steering* getSteering(a3vec2 diff);

	float mTargetAngle = 0.1f;
	float mSlowAngle = 0.5f;
	float mTimeToTarget = 0.1f;

	float mTargetOrient;
	float mRotation;
	float mRotationSize;

	float mTargetRotation;
	float mAngularAcc;
	//float rotationDirection;
};