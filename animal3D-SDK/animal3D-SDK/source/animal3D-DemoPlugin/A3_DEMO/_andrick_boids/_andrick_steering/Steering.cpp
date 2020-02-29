#include "Steering.h"


void Steering::update()
{
	Steering* pTemp = getSteering();
	if (pTemp != 0)
	{
		mData = pTemp->getData();
	}
}
