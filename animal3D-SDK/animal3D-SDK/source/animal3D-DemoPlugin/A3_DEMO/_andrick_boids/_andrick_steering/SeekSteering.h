#pragma once
#include "Steering.h"

class SeekSteering : public Steering
{
public:
	SeekSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();
};