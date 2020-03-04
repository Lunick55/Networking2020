#include <cassert>

#include "Steering.h"
#include "SeparationSteering.h"
#include <A3_DEMO/_andrick_boids/andrick_boid_manager.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>



SeparationSteering::SeparationSteering(const UnitID& ownerID, const a3vec2& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mSeekSteering(SeekSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEPARATION;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeparationSteering::getSteering()
{
	a3vec2 diff;
	Boid* pOwner = gDemoState->mpBoidManager->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	a3vec2 positionVector;
	int neighborCount = 0;
	mNeighborRadius = gDemoState->mSeparationRadius;

	std::map<UnitID, Boid*> unitMap = gDemoState->mpBoidManager->getMap();

	for (auto it = unitMap.begin(); it != unitMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			a3vec2 unitPos = it->second->getPositionComponent()->getPosition();
			a3real2Diff(diff.v, unitPos.v, pOwner->getPositionComponent()->getPosition().v);
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
	a3real2DivS(positionVector.v, (float)neighborCount);

	mSeekSteering.setTargetLoc(positionVector);
	mSeekSteering.getSteering();

	data.rotAcc = 0;
	data.rotVel = 0;
	a3real2ProductS(data.acc.v, mSeekSteering.getData().acc.v, -1.0f);

	this->mData = data;
	return this;
}

