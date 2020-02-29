#include <A3_DEMO/_andrick_boids/andrick_boid_physics.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>

void BoidPhysics::modData(const PhysicsData& data)
{
	mData.vel.x += data.vel.x;
	mData.vel.y += data.vel.y;
	mData.acc.x += data.acc.x;
	mData.acc.y += data.acc.y;
	mData.rotVel += data.rotVel;
	mData.rotAcc += data.rotAcc;
}

void BoidPhysics::integrate(BoidPosition& posComponent, float elapsedTime)
{
	//make sure elapsedTime is at least 0.0f
	//assert(elapsedTime >= 0.0f);
	if (elapsedTime < 0.0f)
	{
		elapsedTime = 0.0f;
	}

	if (elapsedTime > 0.0f)//no need to do this for no elapsed time
	{
		//cap accelerations
		if (a3real2LengthSquared(mData.acc.v) > (mData.maxAccMagnitude * mData.maxAccMagnitude))
		{
			a3real2Normalize(mData.acc.v);
			mData.acc.x *= mData.maxAccMagnitude;
			mData.acc.y *= mData.maxAccMagnitude;
		}
		if (mData.rotAcc > mData.maxRotAcc)
		{
			mData.maxRotAcc = mData.maxRotAcc;
		}
		else if (mData.rotAcc < -mData.maxRotAcc)
		{
			mData.rotAcc = -mData.maxRotAcc;
		}

		//apply accelerations to velocities
		mData.vel.x += (mData.acc.x * elapsedTime);
		mData.vel.y += (mData.acc.y * elapsedTime);
		mData.rotVel += (mData.rotAcc * elapsedTime);

		//cap velocities
		if (a3real2LengthSquared(mData.vel.v) > (mData.maxSpeed * mData.maxSpeed))
		{
			a3real2Normalize(mData.vel.v);
			mData.vel.x *= mData.maxSpeed;
			mData.vel.y *= mData.maxSpeed;
			mData.acc = a3vec2_zero;
		}
		if (mData.rotVel > mData.maxRotVel)
		{
			mData.rotVel = mData.maxRotVel;
			mData.rotAcc = 0.0f;
		}
		else if (mData.rotVel < -mData.maxRotVel)
		{
			mData.rotVel = -mData.maxRotVel;
			mData.rotAcc = 0.0f;
		}

		//apply velocities to position component
		a3vec2 tVec;
		a3real2ProductS(tVec.v, mData.vel.v, elapsedTime);
		posComponent.modPosition(tVec);
		posComponent.modFacing(mData.rotVel * elapsedTime);
	}

}