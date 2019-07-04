#include "DynamicObject.h"
#include "Helper.h"

#include <math.h>
DynamicObject::DynamicObject(std::string objectName, Vector2f centerPosition) : WorldObject(objectName, centerPosition)
{
	speed = 0;
	direction = STAND;
}

DynamicObject::~DynamicObject()
{

}

void DynamicObject::handleInput()
{
}

void DynamicObject::setSide(Vector2f otherObjectPosition, float elapsedTime)
{
	Side answer = down;

	float alpha = atan((float(otherObjectPosition.y) - this->position.y) / (float(otherObjectPosition.x) - this->position.x)) * 180 / pi;
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

void DynamicObject::setMoveOffset(float elapsedTime)
{
	if (movePosition == Vector2f(-1, -1))
	{
		if (direction != STAND)
		{
			auto angle = this->getDirection() * pi / 180;

			moveOffset.x = float(this->speed * cos(angle));
			moveOffset.y = float(this->speed * -sin(angle));
			return;
		}
		moveOffset = Vector2f(-1, -1);
		return;
	}

	float distanceToTarget = float(sqrt(pow(movePosition.x - position.x, 2) + pow(movePosition.y - position.y, 2)));

	if (distanceToTarget <= radius)
	{
		moveOffset = Vector2f (-1, -1);
		return;
	}

	moveOffset = Vector2f ((movePosition.x - position.x) * speed / distanceToTarget, (movePosition.y - position.y) * speed / distanceToTarget);

	Vector2f curPos = this->getPosition();
	Vector2f tarPos = movePosition;

	if (abs(curPos.x - tarPos.x) < (this->radius))
	{
		if (curPos.y < tarPos.y)
		{
			direction = DOWN;
			return;
		}
		direction = UP;
		return;
	}
	if (abs(curPos.y - tarPos.y) < (this->radius))
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

bool DynamicObject::isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic)
{
	//Vector2f position1 = dynamic1.getPosition();
	Vector2f position1 = newPosition;
	Vector2f position2 = otherDynamic.getPosition();
	//return (abs(position1.x - position2.x) <= (this->getRadius() + otherDynamic.getRadius()) && abs(position1.y - position2.y) <= (this->getRadius() + otherDynamic.getRadius()));
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= /*this->radius + */otherDynamic.radius;
}

Vector2f DynamicObject::EllipceSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, float elapsedTime)
{
	Vector2f terrainPos = Vector2f((f1.x + f2.x) / 2, ((height - f1.y) + (height - f2.y)) / 2);
	Vector2f dynamicPos = Vector2f(dynamic->getPosition().x, height - dynamic->getPosition().y);
	Vector2f focus1 = Vector2f(f1.x, height - f1.y);
	Vector2f focus2 = Vector2f(f2.x, height - f2.y);
	Vector2f newPosition = Vector2f(newPos.x, height - newPos.y);
	Vector2f destinatonPos = Vector2f(destination.x, height - destination.y);

	float a2, b2, c1 = 0, x0 = terrainPos.x, y0 = terrainPos.y;

	float a1 = (newPosition.x - dynamicPos.x);
	float b1 = (newPosition.y - dynamicPos.y);
	c1 = -a1 * newPosition.x - b1 * newPosition.y;

	float l = (ellipseSize - abs(focus2.x - focus1.x)) / 2;
	a2 = ellipseSize / 2;
	b2 = sqrt(pow((2 * l + (abs(focus2.x - focus1.x))) / 2, 2) - pow(abs(focus2.x - focus1.x) / 2, 2));
	x0 = (focus1.x + focus2.x) / 2;
	y0 = (focus1.y + focus2.y) / 2;

	float x1, yl, x2, y2, D = 0;

	if (a1 == 0 && b1 != 0)
	{
		D = pow(2 * x0*b1*b1*b2*b2, 2) - 4 * (b1*b1*b2*b2)*(x0*x0*b1*b1*b2*b2 + c1 * c1*a2*a2 + y0 * y0*a2*a2*b1*b1 + 2 * c1*y0*a2*a2*b1 - a2 * a2*b1*b1*b2*b2);

		if (b1*b1*b2*b2 == 0 || D < 0)
			return { -1, -1 };

		x1 = -((-(2 * x0*b1*b1*b2*b2) + sqrt(D)) / (2 * (b1*b1*b2*b2)));
		x2 = -((-(2 * x0*b1*b1*b2*b2) - sqrt(D)) / (2 * (b1*b1*b2*b2)));
		yl = -c1 / b1;
		y2 = -c1 / b1;
	}
	else
		if (a1 != 0 && b1 == 0)
		{
			D = pow(-2 * y0*a1*a1*a2*a2, 2) - 4 * (a1*a1*a2*a2)*(c1*c1*b2*b2 + x0 * x0*a1*a1*b2*b2 + 2 * c1*x0*a1*b2*b2 + y0 * y0*a1*a1*a2*a2 - a1 * a1*a2*a2*b2*b2);

			if (a1*a1*a2*a2 == 0 || D < 0)
				return { -1, -1 };

			yl = (2 * y0*a1*a1*a2*a2 + sqrt(D)) / (2 * a1*a1*a2*a2);
			y2 = (2 * y0*a1*a1*a2*a2 - sqrt(D)) / (2 * a1*a1*a2*a2);
			x1 = -c1 / a1;
			x2 = -c1 / a1;
		}
		else
			if (a1 != 0 && b1 != 0)
			{
				D = pow(2 * b1*b2*b2*c1 + 2 * a1*b1*b2*b2*x0 - 2 * a1*a1*a2*a2*y0, 2) - 4 * (b1*b1*b2*b2 + a1 * a1*a2*a2)*(b2*b2*c1*c1 + a1 * a1*b2*b2*x0*x0 + 2 * a1*b2*b2*c1*x0 + a1 * a1*a2*a2*y0*y0 - a1 * a1*a2*a2*b2*b2);

				if (b1*b1*b2*b2 + a1 * a1*a2*a2 == 0 || D < 0)
					return { -1, -1 };

				yl = (-(2 * b1*b2*b2*c1 + 2 * a1*b1*b2*b2*x0 - 2 * a1*a1*a2*a2*y0) + sqrt(D)) / (2 * (b1*b1*b2*b2 + a1 * a1*a2*a2));
				y2 = (-(2 * b1*b2*b2*c1 + 2 * a1*b1*b2*b2*x0 - 2 * a1*a1*a2*a2*y0) - sqrt(D)) / (2 * (b1*b1*b2*b2 + a1 * a1*a2*a2));
				x1 = -1 * (b1 * yl + c1) / a1;
				x2 = -1 * (b1 * y2 + c1) / a1;
			}
			else
				return { -1, -1 };

	float dist1, dist2;

	if (destination == Vector2f(-1, -1))
		dist1 = Helper::getDist(newPosition, Vector2f(x1, yl)), dist2 = Helper::getDist(newPosition, Vector2f(x2, y2));
	else
		dist1 = Helper::getDist(destinatonPos, Vector2f(x1, yl)), dist2 = Helper::getDist(destinatonPos, Vector2f(x2, y2));

	if (dist1 <= dist2 && D >= 0 && sqrt(pow(x1 - dynamic->getPosition().x, 2) + pow((height - yl) - dynamic->getPosition().y, 2)) != 0)
	{
		float k = ((dynamic->getSpeed() * elapsedTime) / sqrt(pow(x1 - dynamic->getPosition().x, 2) + pow((height - yl) - dynamic->getPosition().y, 2)));
		return { (x1 - dynamic->getPosition().x) * k, ((height - yl) - dynamic->getPosition().y) * k };
	}
	if (dist2 <= dist1 && D >= 0 && sqrt(pow(x2 - dynamic->getPosition().x, 2) + pow((height - y2) - dynamic->getPosition().y, 2)) != 0)
	{
		float k = ((dynamic->getSpeed() * elapsedTime) / sqrt(pow(x2 - dynamic->getPosition().x, 2) + pow((height - y2) - dynamic->getPosition().y, 2)));
		return { (x2 - dynamic->getPosition().x) * k, ((height - y2) - dynamic->getPosition().y) * k };
	}

	return { -1, -1 };
}

Vector2f DynamicObject::doMove(long long elapsedTime)
{
	setMoveOffset(elapsedTime);

	if (this->direction == STAND)
		return Vector2f(this->position);

	auto position = this->position;
	auto moveOffset = this->moveOffset;

	if (moveOffset != Vector2f(-1, -1))
	{
		position.x += moveOffset.x * elapsedTime;
		position.y += moveOffset.y * elapsedTime;

		return Vector2f(position);
	}

	return Vector2f(position);
}

Vector2f DynamicObject::doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, float elapsedTime)
{
	bool crashed = false;

	for (auto staticItem : localStaticItems)
	{
		auto terrain = dynamic_cast<TerrainObject*>(staticItem);
		if (!terrain || staticItem->isBackground)
			continue;

		if (terrain->isMultiellipse)
		{
			std::vector<int> curEllipses = terrain->getMultiellipseIntersect(newPosition);
			Vector2f motionAfterSlipping;

			if (!curEllipses.empty())
				for (auto curEllipse : curEllipses)
				{
					if (crashed)
						return Vector2f(-1, -1);

					crashed = true;
					Vector2f ellipsePos = terrain->internalEllipses[curEllipse].first.second;
					motionAfterSlipping = this->EllipceSlip(this, newPosition, this->movePosition, terrain->internalEllipses[curEllipse].second.first, terrain->internalEllipses[curEllipse].second.second, terrain->internalEllipses[curEllipse].first.first, height, elapsedTime);

					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						newPosition = Vector2f(this->getPosition().x + motionAfterSlipping.x, this->getPosition().y + motionAfterSlipping.y);
						continue;
					}

					return Vector2f(-1, -1);
				}
		}
		else
			if (terrain->isIntersected(this->getPosition(), this->getRadius(), newPosition))
			{
				if (crashed)
					return Vector2f(-1, -1);

				crashed = true;				
				Vector2f motionAfterSlipping;

				if (staticItem->isDotsAdjusted)
					motionAfterSlipping = terrain->newSlippingPositionForDotsAdjusted(this->getPosition(), moveOffset, this->getSpeed(), elapsedTime);
				else
					motionAfterSlipping = this->EllipceSlip(this, newPosition, this->movePosition, terrain->getFocus1(), terrain->getFocus2(), terrain->getEllipseSize(), height, elapsedTime);

				if (motionAfterSlipping != Vector2f(-1, -1))
				{
					newPosition = Vector2f(this->getPosition().x + motionAfterSlipping.x, this->getPosition().y + motionAfterSlipping.y);
					continue;
				}

				return Vector2f(-1, -1);
			}
	}

	return newPosition;
}

Vector2f DynamicObject::doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, float elapsedTime)
{
	if (!canCrashIntoDynamic)
		return newPosition;

	lastIntersected = "";

	for (auto otherDynamicItem : localDynamicItems)
	{
		if (otherDynamicItem == this)
			continue;

		if (this->isIntersectDynamic(newPosition, *otherDynamicItem) && otherDynamicItem->lastIntersected != name)
		{
			this->lastIntersected = otherDynamicItem->getName();
			auto motionAfterSlipping = EllipceSlip(this, newPosition, movePosition, otherDynamicItem->getPosition(), otherDynamicItem->getPosition(), otherDynamicItem->getRadius() * 2, height, elapsedTime);

			if (motionAfterSlipping != Vector2f(-1, -1))
			{
				newPosition = Vector2f(position.x + motionAfterSlipping.x, position.y + motionAfterSlipping.y);
				continue;
			}

			return Vector2f(-1, -1);
		}
	}

	return newPosition;
}

void DynamicObject::changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
}

void DynamicObject::takeDamage(float damage)
{
	this->timeForNewHitself = 0;
	this->healthPoint -= damage / this->armor;
}

std::string DynamicObject::sideToString(Side side)
{
	switch (side)
	{
		case up:
			return "up";
		case right:
			return "right";
		case down:
			return "down";
		case  left:
			return "left";
	}
	return "";
}

std::string DynamicObject::directionToString(Direction direction)
{
	switch (direction)
	{
	case UP:
		return "up";
	case RIGHT:
		return "right";
	case DOWN:
		return "down";
	case LEFT:
		return "left";
	case UPRIGHT:
		return "right";
	case UPLEFT:
		return "left";
	case DOWNRIGHT:
		return "right";
	case DOWNLEFT:
		return "left";
	}
	return "";
}

