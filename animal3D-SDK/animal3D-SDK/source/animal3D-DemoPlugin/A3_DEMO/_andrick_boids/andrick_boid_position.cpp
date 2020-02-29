#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>

void BoidPosition::wrapCoords()
{
	if (mWrap)
	{
		//TODO: Implement screen wrap. Dunno how to get screen info
		//GraphicsSystem* pGraphics = gpGame->getGraphicsSystem();
		//float width = (float)pGraphics->getWidth();
		//float height = (float)pGraphics->getHeight();
		//
		//float x = mData.pos.getX();
		//float y = mData.pos.getY();
		//
		//if (x < 0.0f)
		//{
		//	mData.pos.setX(width);
		//}
		//else if (x > width)
		//{
		//	mData.pos.setX(0.0f);
		//}
		//
		//if (y < 0.0f)
		//{
		//	mData.pos.setY(height);
		//}
		//else if (y > height)
		//{
		//	mData.pos.setY(0.0f);
		//}
	}
}
