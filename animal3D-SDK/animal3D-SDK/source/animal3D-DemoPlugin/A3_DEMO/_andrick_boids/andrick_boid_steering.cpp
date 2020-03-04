#include <A3_DEMO/_andrick_boids/andrick_boid_steering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/SeekSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/ArriveSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/WanderSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/FaceSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/CohesionSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/SeparationSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/GroupAlignSteering.h>
#include <A3_DEMO/_andrick_boids/_andrick_steering/FlockingSteering.h>

BoidSteering::BoidSteering()
	: mpSteering(nullptr)
{
}

BoidSteering::~BoidSteering()
{
	delete mpSteering;
}

void BoidSteering::applySteering(BoidPhysics& physicsComponent)
{
	if (mpSteering != nullptr)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void BoidSteering::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
	case Steering::SEEK:
	{
		//cleanup old steering - todo: check for already existing steering and reuse if possible
		delete mpSteering;
		//create new steering
		mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::FLEE:
	{
		delete mpSteering;

		mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, true);
		break;
	}
	case Steering::ARRIVE:
	{
		delete mpSteering;

		mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::FACE:
	{
		delete mpSteering;

		mpSteering = new FaceSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::WANDER:
	{
		delete mpSteering;

		mpSteering = new WanderSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::COHESION:
	{
		delete mpSteering;

		mpSteering = new CohesionSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::SEPARATION:
	{
		delete mpSteering;

		mpSteering = new SeparationSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::GROUPALIGN:
	{
		delete mpSteering;

		mpSteering = new GroupAlignSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::FLOCKING:
	{
		delete mpSteering;

		mpSteering = new FlockingSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	default:
	{
		
	}
	};
}

