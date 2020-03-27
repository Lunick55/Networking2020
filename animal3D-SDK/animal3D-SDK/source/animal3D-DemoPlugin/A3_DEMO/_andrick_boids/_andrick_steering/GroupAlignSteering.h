#pragma once
#include "Steering.h"
#include "FaceSteering.h"

class GroupAlignSteering : public Steering
{
public:
	GroupAlignSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	FaceSteering mFaceSteering;
	float mNeighborRadius;
};