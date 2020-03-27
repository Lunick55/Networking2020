#ifndef _STEERING_H
#define _STEERING_H

/*Steering - implementation of Steering class from "Artificial Intelligence for Games" - Millington and Funge
*/

#include <animal3D-A3DM/a3math/a3vector.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_physics.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <Bits.h>
//#include "defines.h"

//const IDType INVALID_ID = -1;
//const IDType HIGHEST_ID = 9999;
const UnitID INVALID_UNIT_ID = UINT_MAX;

class Steering
{
public:
	enum SteeringType
	{
		INVALID_TYPE = -1,
		SEEK,
		FLEE,
		ARRIVE,
		FACE,
		WANDER,
		WANDERCHASE,
		ARRIVEFACE,
		COHESION,
		SEPARATION,
		GROUPALIGN,
		FLOCKING
	};

	//constructors and destructors
	Steering(UserId userId, SteeringType type = INVALID_TYPE ):mType(type), mTargetLoc(a3vec2_zero), mTargetID(INVALID_UNIT_ID),
		mUserID(userId), mOwnerID(INVALID_UNIT_ID), mData(ZERO_PHYSICS_DATA) {};
	virtual ~Steering(){};

	//accessors
	//a3vec2 this should be a vector 2
	void setTargetLoc(const a3vec2& loc) { mTargetLoc = loc; };
	const a3vec2& getTargetLoc() const { return mTargetLoc; };
	void setTargetID(const UnitID& id) { mTargetID = id; };
	const UnitID& getTargetID() const { return mTargetID; };
	void setOwnerID(const UnitID& id) { mOwnerID = id; };
	void setData(const PhysicsData& data) { mData = data; };
	const PhysicsData& getData() const { return mData; };
	void update();
protected:
	SteeringType mType;
	a3vec2 mTargetLoc;
	UnitID mTargetID;
	UnitID mOwnerID;
	PhysicsData mData;
	UserId mUserID;

	virtual Steering* getSteering() { return this; };//overridden by sub-classes

};


#endif // !_STEERING_H