#include "DynamicObject.h"

DynamicObject::DynamicObject(std::string objectName, Vector2f centerPosition) : WorldObject(objectName, centerPosition)
{
	speed = 0;
	direction = STAND;	
}

void DynamicObject::handleInput()
{
}

void DynamicObject::takeDamage(float damage)
{
	this->timeForNewHitself = 0;
	this->healthPoint -= damage / this->armor;
}

void DynamicObject::setSide(Vector2f otherObjectPosition)
{
	Side answer;
	float alpha = atan((otherObjectPosition.y - this->position.y) / (otherObjectPosition.x - this->position.x)) * 180 / pi;
	if (this->position.y >= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = up;
	else
		if (this->position.x <= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = right;
		else
			if (this->position.y <= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = down;
			else
				if (this->position.x >= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = left;
	side = answer;
}

void DynamicObject::moveToTarget(float targetRadius)
{
	double distanceToTarget = sqrt(pow(targetPosition.x - position.x, 2) + pow(targetPosition.y - position.y, 2));

	if (distanceToTarget <= targetRadius)
	{
		moveOffset = Vector2f(-1, -1);
		return;
	}

	moveOffset = Vector2f((targetPosition.x - position.x) * speed / distanceToTarget, (targetPosition.y - position.y) * speed / distanceToTarget);

	Vector2f curPos = this->getPosition();
	Vector2f tarPos = targetPosition;
	if (abs(curPos.x - tarPos.x) < (this->radius + targetRadius))
	{
		if (curPos.y < tarPos.y)
		{
			direction = DOWN;
			return;
		}
		direction = UP;
		return;
	}
	if (abs(curPos.y - tarPos.y) < (this->radius + targetRadius))
	{
		if (curPos.x < tarPos.x)
		{
			direction = RIGHT;
			return;
		}
		direction = LEFT;
		return;
	}
	if (curPos.x < tarPos.x)
	{
		if (curPos.y < tarPos.y)
		{
			direction = DOWNRIGHT;
		}
		else
			if (curPos.y > tarPos.y)
			{
				direction = UPRIGHT;
			}
	}
	else
		if (curPos.x > tarPos.x)
		{
			if (curPos.y < tarPos.y)
			{
				direction = DOWNLEFT;
			}
			else
				if (curPos.y > tarPos.y)
				{
					direction = UPLEFT;
				}
		}
}



