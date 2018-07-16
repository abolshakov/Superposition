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

void Creature::behavior(DynamicObject& target, float elapsedTime)
{
	if (healthPoint <= 0)
	{
		currentAction = dead;
		direction = STAND;
		return;
	}

	if (target.tag != "player")
		return;

	setSide(target.getPosition());

	srand(time(0));

	Vector2f curPos = this->getPosition();
	Vector2f tarPos = target.getPosition();

	float distanceToTarget = sqrt(pow(curPos.x - tarPos.x, 2) + pow(curPos.y - tarPos.y, 2));

	if (distanceToTarget <= sightRange)
	{
		currentAction = move;
		
		fear = target.getStrength() * target.getRadius();
		int randomOffsetX = rand() % (fear / morality) - (fear / morality) / 2;
		int randomOffsetY = rand() % (fear / morality) - (fear / morality) / 2;

		targetPosition = Vector2f(position.x - (target.getPosition().x - position.x + randomOffsetX), position.y - (target.getPosition().y - position.y + randomOffsetY));
		moveToTarget(target.getRadius());
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