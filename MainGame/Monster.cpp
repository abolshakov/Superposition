#include "Monster.h"

using namespace sf;

Monster::Monster(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	currentAction = relax;
	sightRange = 600;
	strikingSprite = 6;
	timeForNewHit = 1e5;
}

Monster::~Monster()
{

}

void Monster::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void Monster::behavior(float elapsedTime)
{
	endingPreviousAction();
	fightLogic(elapsedTime);
}

void Monster::setTarget(DynamicObject& object)
{
	victim = nullptr;
	if (object.tag == Tag::hero1)
		return; //targetPosition = object.getPosition();
}

void Monster::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}

	if (target->tag != Tag::hero1)
		return;

	victim = target;
	side = calculateSide(victim->getPosition(), elapsedTime);

	if (Helper::getDist(position, victim->getPosition()) <= sightRange && timeAfterHit >= timeForNewHit)
		speed = std::max((1 - Helper::getDist(position, victim->getPosition()) / sightRange) * defaultSpeed + defaultSpeed, defaultSpeed);
	else
		speed = defaultSpeed;
	if (isAttack.count(currentAction) == 0)
		timeAfterHit += elapsedTime;

	// fight with player	
	doAttack(victim);		
	if (Helper::getDist(position, victim->getPosition()) <= (this->radius + victim->getRadius() + hitDistance * 1.3) &&
		isAttack.count(currentAction) > 0 && currentSprite[0] == strikingSprite && !wasHit)
	{
		victim->takeDamage(this->strength, position);
		wasHit = true;
	}

	if (isAttack.count(currentAction) == 0 && Helper::getDist(position, victim->getPosition()) <= (this->radius + victim->getRadius() + hitDistance / 5))
	{
		stopping(true, false);
		changeAction(combatState, false, false);
		wasHit = false;									
	}
	//---------------------
	
	// move to player
	if (Helper::getDist(position, victim->getPosition()) > (this->radius + victim->getRadius() + hitDistance / 5))
	{
		if (isAttack.count(currentAction) == 0 && currentAction != combatState)
		{
			changeAction(move, false);
			movePosition = victim->getPosition();
		}
	}
	//---------------
}

void Monster::endingPreviousAction()
{
	if (lastAction == combatState)
		changeAction(relax, true, false);
	if (lastAction == commonHit)
		changeAction(combatState, true, false);
	if (lastAction == directHit)
		changeAction(combatState, true, false);
	lastAction = relax;
}

void Monster::stopping(bool doStand, bool forgetSelectedTarget)
{
	if (doStand)
	{
		this->movePosition = { -1, -1 };
		moveOffset = { 0, 0 };
		this->direction = STAND;
	}

	if (forgetSelectedTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}
}

Vector2f Monster::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Monster::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Monster::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Monster::fightLogic(float elapsedTime, DynamicObject* target)
{	
	pushAway(elapsedTime);
}
