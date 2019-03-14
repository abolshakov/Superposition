#include "NeutralMob.h"

using namespace sf;

NeutralMob::NeutralMob(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	currentAction = relax;
}

NeutralMob::~NeutralMob()
{

}

void NeutralMob::setTarget(DynamicObject& object)
{
	return;
}

void NeutralMob::behaviorWithDynamic(DynamicObject& target, float elapsedTime)
{
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}

	setSide(movePosition, elapsedTime);

	const Vector2f curPos = this->getPosition();
	const Vector2f tarPos = target.getPosition();

	const float distanceToTarget = Helper::getDist(curPos, tarPos);

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		
		fear = target.getStrength() * target.getRadius();
		const int randomOffsetX = rand() % (fear / morality) - (fear / morality) / 2;
		const int randomOffsetY = rand() % (fear / morality) - (fear / morality) / 2;

		movePosition = Vector2f(position.x - (target.getPosition().x - position.x + randomOffsetX), position.y - (target.getPosition().y - position.y + randomOffsetY));
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
				movePosition = Vector2f(position.x - (target.getPosition().x - position.x), position.y - (target.getPosition().y - position.y));
		}
	}
}

void NeutralMob::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void NeutralMob::behavior(float elapsedTime)
{

}