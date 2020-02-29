#pragma once

#include <animal3D-A3DM/a3math/a3vector.h>

struct PositionData
{
	PositionData() :pos(a3vec2_zero), facing(0.0f) {};
	PositionData(const a3vec2& thePos, float theFacing) :pos(thePos), facing(theFacing) {};

	a3vec2 pos;
	float facing;
};

const PositionData ZERO_POSITION_DATA;

class BoidPosition
{
public:
	BoidPosition(bool shouldWrap = true) : mWrap(shouldWrap) {};
	~BoidPosition() {};

	//getters and setters
	const a3vec2& getPosition() const { return mData.pos; };
	void setPosition(const a3vec2& pos) { mData.pos = pos; wrapCoords(); };
	void modPosition(const a3vec2& mod) { mData.pos.x += mod.x; mData.pos.y += mod.y; wrapCoords(); };
	float getFacing() const { return mData.facing; };
	void setFacing(float facing) { mData.facing = facing; };
	void modFacing(float mod) { mData.facing += mod; };
	const PositionData& getData() const { return mData; };
	void setData(const PositionData& data) { mData = data; wrapCoords(); };
	void modData(const PositionData& data) { mData.pos.x += data.pos.x; mData.pos.y += data.pos.y; mData.facing += data.facing; wrapCoords(); };

private:
	PositionData mData;
	bool mWrap;//should we wrap around the screen?


	void wrapCoords();

	friend class BoidManager;
};