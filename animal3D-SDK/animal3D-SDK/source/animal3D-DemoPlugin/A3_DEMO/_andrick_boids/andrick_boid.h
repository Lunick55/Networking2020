#pragma once

#include <animal3D-A3DM/a3math/a3vector.h>
//#include "PositionComponent.h"
//#include "Sprite.h"
#include <A3_DEMO/_andrick_boids/_andrick_steering/Steering.h>
#include <a3_dylib_config_export.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>

class BoidPosition;
class BoidSteering;
class UnitManager;

const a3ui32 DEFAULT_QUEUE_CAPACITY = 8;


//class Unit : public TransactionProcessor
class Boid
{
public:
	void draw() const;
	float getFacing() const;
	void update(float elapsedTime);

	BoidPosition* getPositionComponent() const;
	BoidPhysics* getPhysicsComponent() const;
	BoidSteering* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };

	void setSteering(Steering::SteeringType type, a3vec2 targetLoc = a3vec2_zero, UnitID targetUnitID = INVALID_UNIT_ID);

private:
	UnitID mID;
	BoidPosition* mpPosition;
	BoidPhysics* mpPhysics;
	BoidSteering* mpSteering;
	//Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	Boid();// const Sprite& sprite);
	virtual ~Boid();

	//Boid(Boid&);//invalidate copy constructor
	//void operator=(Boid&);//invalidate assignment operator

	friend class BoidManager;
};
