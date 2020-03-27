#pragma once
#include "Steering.h"
#include "SeekSteering.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	SeekSteering mSeekSteering;
	float mNeighborRadius;
};