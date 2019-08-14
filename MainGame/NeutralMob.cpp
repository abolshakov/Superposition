#include "NeutralMob.h"

using namespace sf;

NeutralMob::NeutralMob(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite[0] = 1;
	currentAction = relax;
}

NeutralMob::~NeutralMob()
{

}

void NeutralMob::setTarget(DynamicObject& object)
{
	if (object.tag == Tag::noose)
		return;
	if (Helper::getDist(position, object.getPosition()) < distanceToNearest)
	{
		boundTarget = &object;
		distanceToNearest = Helper::getDist(position, object.getPosition());
	}
}

void NeutralMob::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	
}

void NeutralMob::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void NeutralMob::behavior(float elapsedTime)
{
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}

	setSide(movePosition, elapsedTime);	
	//return;
	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				direction = STAND;
				movePosition = { -1, -1 };
			}
			else
				movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
		}
	}

	distanceToNearest = 10e6;
	boundTarget = nullptr;
}