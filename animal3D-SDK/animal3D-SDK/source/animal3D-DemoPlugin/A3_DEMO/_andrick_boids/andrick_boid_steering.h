#pragma once

#include <animal3D-A3DM/a3math/a3vector.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

class BoidPhysics;
class Steering;
enum Steering::SteeringType;

struct SteeringData
{
	SteeringData() :type(Steering::INVALID_TYPE), targetLoc(a3vec2_zero), ownerID(INVALID_UNIT_ID), targetID(INVALID_UNIT_ID) {};
	SteeringData(Steering::SteeringType theType, const a3vec2& theTargetLoc = a3vec2_zero, UnitID theOwner = INVALID_UNIT_ID, UnitID theTarget = INVALID_UNIT_ID)
		:type(theType), targetLoc(theTargetLoc), ownerID(theOwner), targetID(theTarget) {};
	Steering::SteeringType type;
	a3vec2 targetLoc;
	UnitID ownerID;
	UnitID targetID;
};

const SteeringData ZERO_STEERING_DATA;

class BoidSteering
{
public:
	//getters and setters
	Steering::SteeringType getType() const { return mData.type; };
	const a3vec2& getTargetLoc() const { return mData.targetLoc; };
	UnitID getOwnerID() const { return mData.ownerID; };
	UnitID getTargetID() const { return mData.targetID; };
	BoidPhysics* getPhysicsComponent() const { return mpPhysicsComponent; };
	const SteeringData& getData() const { return mData; };
	void setData(const SteeringData& data);
	void setTargetLoc(const a3vec2& pos) { mData.targetLoc = pos; };
	void setTargetUnitID(const UnitID& id) { mData.targetID = id; };
	void applySteering(BoidPhysics& physicsComponent);
private:
	BoidPhysics* mpPhysicsComponent;
	SteeringData mData;
	Steering* mpSteering;

	BoidSteering();
	~BoidSteering();
	void setPhysicsComponent(BoidPhysics* pComp) { mpPhysicsComponent = pComp; };

	friend class BoidManager;
};