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

	Boid* createUnit(const UserId& userId, bool shouldWrap = true, const SteeringData& steerData = ZERO_STEERING_DATA, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Boid* createPlayerUnit(const UserId& userId, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Boid* createRandomUnit(const UserId& userId);

	bool isUserRegistered(const UserId& userId) const { return mUnitMap.find(userId) != mUnitMap.end(); }
	Boid* getUnit(const UserId& userId, const UnitID& unitId) const;
	void deleteUnit(const UserId& userId, const UnitID& id);
	void deleteRandomUnit(const UserId& userId);

	void drawAll() const;
	void updateAll(float elapsedTime);

	Boid* getPlayerUnit(const UserId& userId) const;

	std::map<UserId, std::map<UnitID, Boid*>> getMap() { return mUnitMap; };

private:
	static UnitID msNextUnitID;
	std::map<UserId, std::map<UnitID, Boid*>> mUnitMap;
};