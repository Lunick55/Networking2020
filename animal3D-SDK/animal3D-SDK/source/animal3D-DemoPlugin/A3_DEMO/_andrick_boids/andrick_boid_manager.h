#pragma once

#include <map>
#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_physics.h>
#include <A3_DEMO/_andrick_boids/andrick_boid.h>

class Boid;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;
const float PI = 3.14159f;

class BoidManager
{
public:
	BoidManager();
	~BoidManager() {};

	Boid* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Boid* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Boid* createRandomUnit(const Sprite& sprite);

	Boid* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	Boid* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	std::map<UnitID, Boid*> getMap() { return mUnitMap; };

private:
	static UnitID msNextUnitID;
	std::map<UnitID, Boid*> mUnitMap;
};