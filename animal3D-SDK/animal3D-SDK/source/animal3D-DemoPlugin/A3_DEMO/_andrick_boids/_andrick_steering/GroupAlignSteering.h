#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class GroupAlignSteering : public Steering
{
public:
	GroupAlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	FaceSteering mFaceSteering;
	float mNeighborRadius;
};