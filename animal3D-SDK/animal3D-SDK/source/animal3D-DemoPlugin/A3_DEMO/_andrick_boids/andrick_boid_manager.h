#pragma once

#include <optional>
#include <map>
#include <set>
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

static a3vec2 mult(float f, a3vec2 vec)
{
	a3vec2 newVec;
	newVec.x = f * vec.x;
	newVec.y = f * vec.y;
	return newVec;
}

static a3vec2 div(float f, a3vec2 vec)
{
	const float scale = 1.0f / f;
	return mult(scale, vec);
}

static a3vec2 add(a3vec2 v1, a3vec2 v2)
{
	a3vec2 newVec;
	newVec.x = v1.x + v2.x;
	newVec.y = v1.y + v2.y;
	return newVec;
}

static a3vec2 sub(a3vec2 v1, a3vec2 v2)
{
	a3vec2 newVec;
	newVec.x = v1.x - v2.x;
	newVec.y = v1.y - v2.y;
	return newVec;
}

struct HermiteCubicSpline
{
	a3vec2 startLoc;
	a3vec2 startDerivative;
	a3vec2 targetLoc;
	a3vec2 targetDerivative;

	//From UnrealMathUtility.h
	a3vec2 interpolateLoc(float lerp)
	{
		const float lerpSquared = lerp * lerp;
		const float lerpCubed = lerpSquared * lerp;

		return	add(
					add(
						mult(((2 * lerpCubed) - (3 * lerpSquared) + 1), startLoc), 
						mult((lerpCubed - (2 * lerpSquared) + lerp), startDerivative)
					), 
					add(
						mult((lerpCubed - lerpSquared), targetDerivative), 
						mult(((-2 * lerpCubed) + (3 * lerpSquared)), targetLoc)
					)
				);
	}

	//From UnrealMathUtility.h
	a3vec2 interpolateInterpDerivate(float lerp)
	{
		a3vec2 a =	sub(
						add(
							add(
								mult(6.f, startLoc), 
								mult(3.f, startDerivative)
							), 
							mult(3.f, targetDerivative)
						),
						mult(6.f, targetLoc)
					);

		a3vec2 b =	add (
						sub(
							sub(
								mult(-6.f, startLoc), 
								mult(4.f, startDerivative)
							), 
							mult(2.f, targetDerivative)),
						mult(6.f, targetLoc)
					);

		a3vec2 c = startDerivative;

		const float lerpSquared = lerp * lerp;

		return	add(
					add(
						mult(lerpSquared, a), 
						mult(lerp, b)
					), 
					c
				);
	}
};

struct Move
{
	PhysicsData physicsData;
	PositionData positionData;
	RakNet::Time timeStamp = 0;
};

static bool operator<(const Move& x, const Move& y) {
	return x.timeStamp < y.timeStamp;
}

struct BoidInfo
{
	std::map<UnitID, Move> steeringData;
	std::map<UnitID, std::multiset<Move>> unprocessedMoves;
	float timeSinceLastNetworkUpdate = 0.0f;
	float timeBetweenNetworkUpdates = 0.0f;
};

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
	BoidInfo* getBoidInfo(const UserId& userId)
	{ 
		auto iter = mBoidInfoMap.find(userId);
		BoidInfo* boidInfo = nullptr;

		if (iter != mBoidInfoMap.end())
		{
			boidInfo = iter->second;
		}

		return boidInfo;
	}

private:
	static UnitID msNextUnitID;
	std::map<UserId, std::map<UnitID, Boid*>> mUnitMap = {};
	//std::map<UserId, BoidInfo> mBoidInfoMap = {};

	std::map<UserId, BoidInfo*> mBoidInfoMap = {};

	BoidInfo* createMove(const UserId& userId);
	void simulateMove(const UserId& userId, const BoidInfo& move);
	HermiteCubicSpline createSpline(BoidInfo* boidInfo, Boid* currentBoid);
};