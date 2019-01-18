#include "Creature.h"

using namespace sf;

Creature::Creature(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	currentAction = relax;
}

Creature::~Creature()
{

}

void Creature::setTarget(DynamicObject& object)
{
	return;
}

void Creature::behaviorWithDynamic(DynamicObject& target, float elapsedTime)
{
	if (healthPoint <= 0)
	{
		currentAction = dead;
		direction = STAND;
		return;
	}

	if (target.tag != mainHeroTag)
		return;

	setSide(Vector2i(target.getPosition()), elapsedTime);

	srand(time(0));

	Vector2i curPos = Vector2i(this->getPosition());
	Vector2i tarPos = Vector2i(target.getPosition());

	float distanceToTarget = sqrt(pow(curPos.x - tarPos.x, 2) + pow(curPos.y - tarPos.y, 2));

	if (distanceToTarget <= sightRange)
	{
		currentAction = move;
		
		fear = target.getStrength() * target.getRadius();
		int randomOffsetX = rand() % (fear / morality) - (fear / morality) / 2;
		int randomOffsetY = rand() % (fear / morality) - (fear / morality) / 2;

		movePosition = Vector2f(position.x - (target.getPosition().x - position.x + randomOffsetX), position.y - (target.getPosition().y - position.y + randomOffsetY));
		setMoveOffset(elapsedTime);
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				currentAction = relax;
				direction = STAND;
				currentSprite = 1;
			}
		}
	}
}

void Creature::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Creature::behavior(float elapsedTime)
{

}