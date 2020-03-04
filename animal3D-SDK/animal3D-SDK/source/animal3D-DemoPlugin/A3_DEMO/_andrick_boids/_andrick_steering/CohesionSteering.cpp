#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

#include "Steering.h"
#include "CohesionSteering.h"


CohesionSteering::CohesionSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mArriveSteering(ArriveSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::COHESION;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* CohesionSteering::getSteering()
{
	a3vec2 diff;
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	a3vec2 positionVector;
	int neighborCount = 0;
	mNeighborRadius = gDemoState->mCohesionRadius;

	std::map<UnitID, Boid*> unitMap = gDemoState->mpBoidManager->getMap();

	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			a3vec2 unitPos = it->second->getPositionComponent()->getPosition();
			diff = unitPos;
			a3real2Sub(diff.v, pOwner->getPositionComponent()->getPosition().v);
			if (a3real2Length(diff.v) < mNeighborRadius)
			{
				a3real2Add(positionVector.v, it->second->getPositionComponent()->getPosition().v);
				neighborCount++;
			}
		}
	}

	if (neighborCount == 0)
	{
		//set the values
		data.rotAcc = 0;
		data.rotVel = 0;
		data.acc = a3vec2_zero;
		data.vel = a3vec2_zero;
		this->mData = data;
		return this;
	}
	a3real2DivS(positionVector.v, (a3real)neighborCount);

	mArriveSteering.setTargetLoc(positionVector);
	mArriveSteering.getSteering();

	data.rotAcc = 0;
	data.rotVel = 0;
	data.acc = mArriveSteering.getData().acc;

	this->mData = data;
	return this;
}

