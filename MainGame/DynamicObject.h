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
	Vector2f focus1, focus2;
	int ellipseSize;
	float healthPoint, armor = 1, strength = 0, maxHealthPointValue;	
	void setSide(Vector2f otherObjectPosition);
	Side side;
public:
	int getSpriteNumber() override { return currentSprite; }
	Actions currentAction, lastAction;
	void takeDamage(float damage);
	float getMaxHealthPointValue() { return maxHealthPointValue; }
	float getHealthPoint() { return healthPoint; }
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	float getStrength() { return strength; }
	float speed;
	float radius;
	float timeForNewHitself, timeAfterHitself;
	Direction direction;

	DynamicObject(std::string objectName, Vector2f centerPosition);

	virtual void handleInput();
	
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	
	int getEllipseSize() const { return ellipseSize; }
};

#endif