#pragma once

#include <map>
#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_physics.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_steering.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

class Boid;
struct PositionData;
struct PhysicsData;
struct SteeringData;

const UnitID PLAYER_UNIT_ID = 0;
const float PI = 3.14159f;

class BoidManager
{
public:
	BoidManager();
	~BoidManager() {};

	Boid* createUnit(bool shouldWrap = true, const SteeringData& steerData = ZERO_STEERING_DATA, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Boid* createPlayerUnit(bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Boid* createRandomUnit();

	Boid* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	Boid* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	std::map<UnitID, Boid*> getMap() { return mUnitMap; };

	a3vec2 otherBoids[20];

private:
	static UnitID msNextUnitID;
	std::map<UnitID, Boid*> mUnitMap;
};