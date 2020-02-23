#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "SeekSteering.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	SeekSteering mSeekSteering;
	float mNeighborRadius;
};