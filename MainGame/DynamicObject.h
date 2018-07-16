#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

const float pi = 3.14159265358979323846;

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };
enum Actions { bottomHit = 0, upperHit = 1, directHit = 2, relax = 3, combatState = 4, move = 5, dead = 6, evasionDown = 7, evasionUp = 8,commonHit = 9, hardHit = 10, specialHit = 11, transitionToEnotherWorld = 12, openInventory = 13 };
enum Side { up = 1, right = 2, down = 3, left = 4 };

class DynamicObject : public WorldObject
{
protected:	
	int ellipseSize;
	std::string selectedName;
	Vector2f targetPosition;
	Vector2f focus1, focus2;
	float healthPoint, armor = 1, strength = 0, maxHealthPointValue;	
	float speed;
	float radius;
	float timeAfterHitself;
	void setSide(Vector2f otherObjectPosition);
	Side side;	
	Actions currentAction, lastAction;
	Direction direction;
	Vector2f moveOffset = Vector2f(-1, -1);
public:
	DynamicObject(std::string objectName, Vector2f centerPosition);
	~DynamicObject();
	int getSpriteNumber() override { return currentSprite; }
	int getEllipseSize() const { return ellipseSize; }
	float getMaxHealthPointValue() { return maxHealthPointValue; }
	float getHealthPoint() { return healthPoint; }
	float getRadius() { return radius; }
	float getSpeed() { return speed; }
	float getStrength() { return strength; }
	float getTimeAfterHitself() { return timeAfterHitself; }	
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getMoveOffset() { return moveOffset; }
	Actions getCurrentAction() { return currentAction; }
	Direction getDirection() { return direction; }
	
	void setCurrentAction(Actions action) { this->currentAction = action; }
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	void setSelectedName(std::string name) { selectedName = name; }
	float setTimeAfterHitself(float time) { timeAfterHitself = time; }
	
	void takeDamage(float damage);			
	void moveToTarget(float targetRadius);
	virtual void handleInput();
	virtual void behavior(DynamicObject& target, float elapsedTime) = 0;

	float timeForNewHitself;			
};

#endif