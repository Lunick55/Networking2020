#pragma once
#include "Steering.h"
#include "ArriveSteering.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UserId& userID, const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	ArriveSteering mArriveSteering;
	float mNeighborRadius;
};