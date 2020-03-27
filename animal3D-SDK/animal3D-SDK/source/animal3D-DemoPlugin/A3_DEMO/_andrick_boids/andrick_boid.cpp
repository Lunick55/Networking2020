#include <A3_DEMO/_andrick_boids/andrick_boid.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_physics.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_position.h>
#include <A3_DEMO/_andrick_boids/andrick_boid_steering.h>

//#include "Game.h"
//#include "GraphicsSystem.h"
//#include "Component.h"
//#include "PositionComponent.h"
//#include "PhysicsComponent.h"
//#include "SteeringComponent.h"
//#include "ComponentManager.h"
//#include "SpriteManager.h"


Boid::Boid()//const Sprite& sprite)
	//:mSprite(sprite)
	: mShowTarget(false)
{
}

Boid::~Boid()
{
}

void Boid::update(float elapsedTime)
{
	BoidPhysics* pPhysics = mpSteering->getPhysicsComponent();
	mpSteering->applySteering(*pPhysics);

	BoidPosition* pPos = pPhysics->mpPositionComponent;
	pPhysics->integrate(*pPos, elapsedTime);
}


void Boid::draw() const
{
	BoidPosition* pPosition = getPositionComponent();
	//assert(pPosition != NULL);
	const a3vec2& pos = pPosition->getPosition();
	gTextFormatter.drawBoidText(WHITE, pos);

	if (mShowTarget)
	{
		BoidSteering* pSteering = getSteeringComponent();
		//assert(pSteering != NULL);
		const a3vec2& targetLoc = pSteering->getTargetLoc();
		if (&targetLoc != &a3vec2_zero)
		{
			//TODO: get them sprites and draw functions in!
			//Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			//assert(pTargetSprite != NULL);
			//gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
		}
	}
}

float Boid::getFacing() const
{
	BoidPosition* pPosition = getPositionComponent();
	return pPosition->getFacing();
}

BoidPosition* Boid::getPositionComponent() const
{
	return mpPosition;
}

BoidPhysics* Boid::getPhysicsComponent() const
{
	return mpPhysics;
}

BoidSteering* Boid::getSteeringComponent() const
{
	return mpSteering;
}

void Boid::setSteering(UserId userID, Steering::SteeringType type, a3vec2 targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	BoidSteering* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, userID, targetLoc, mID, targetUnitID));
	}
}