#pragma once
#include <animal3D-A3DM/a3math/a3vector.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>

static const float MAX_ACC = 250.0f;
static const float MAX_SPEED = 220.0f;
static const float MAX_ROT_ACC = 1.0f;
static const float MAX_ROT_VEL = 5.0f;

struct PhysicsData
{
	PhysicsData(
		const a3vec2& theVel
		, const a3vec2& theAcc
		, float theRotVel
		, float theRotAcc
		, float theMaxSpeed = MAX_SPEED
		, float theMaxAcc = MAX_ACC
		, float theMaxRotVel = MAX_ROT_VEL
		, float theMaxRotAcc = MAX_ROT_ACC)
		:vel(theVel)
		, acc(theAcc)
		, rotVel(theRotVel)
		, rotAcc(theRotAcc)
		, maxSpeed(theMaxSpeed)
		, maxAccMagnitude(theMaxAcc)
		, maxRotVel(theMaxRotVel)
		, maxRotAcc(theRotAcc)
	{};

	PhysicsData()
		:vel(a3vec2_zero)
		, acc(a3vec2_zero)
		, rotVel(0.0f)
		, rotAcc(0.0f)
		, maxSpeed(MAX_SPEED)
		, maxAccMagnitude(MAX_ACC)
		, maxRotVel(MAX_ROT_VEL)
		, maxRotAcc(MAX_ROT_ACC)
	{};

	a3vec2 vel;
	a3vec2 acc;
	float rotVel;
	float rotAcc;
	float maxSpeed;
	float maxAccMagnitude;
	float maxRotVel;
	float maxRotAcc;
};

const PhysicsData ZERO_PHYSICS_DATA;

class BoidPhysics
{
public:

	void setData(const PhysicsData& data) { mData = data; };
	void modData(const PhysicsData& data);
	const PhysicsData& getData() const { return mData; };
	const a3vec2& getVelocity() const { return mData.vel; };
	const a3vec2& getAcceleration() const { return mData.acc; };
	float getRotationalVelocity() const { return mData.rotVel; };
	float getRotationalAcceleration() const { return mData.rotAcc; };

private:
	PhysicsData mData;
	BoidPosition* mpPositionComponent = nullptr;

	BoidPhysics(BoidPosition* posComp) : mpPositionComponent(posComp) {};
	~BoidPhysics() {};

	void integrate(BoidPosition& posComponent, float elapsedTime);

	void setPositionComponent(BoidPosition* pComp) { mpPositionComponent = pComp; };

	friend class BoidManager;
};