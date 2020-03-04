#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>
#include <A3_DEMO/_andrick_Demostate/andrick_demostate.h>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>

void BoidPosition::wrapCoords()
{
	if (mWrap)
	{
		float width = (float)gDemoState->windowWidth;
		float height = (float)gDemoState->windowHeight;
		
		float x = mData.pos.x;
		float y = mData.pos.y;
		
		if (x < 0.0f)
		{
			mData.pos.x = width;
		}
		else if (x > width)
		{
			mData.pos.x = 0.0f;
		}
		
		if (y < 0.0f)
		{
			mData.pos.y = height;
		}
		else if (y > height)
		{
			mData.pos.y = 0.0f;
		}
	}
}
