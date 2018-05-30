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




