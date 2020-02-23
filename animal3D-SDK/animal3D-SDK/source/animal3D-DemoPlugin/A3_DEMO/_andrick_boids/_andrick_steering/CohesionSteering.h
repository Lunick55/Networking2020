#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "ArriveSteering.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	ArriveSteering mArriveSteering;
	float mNeighborRadius;
};