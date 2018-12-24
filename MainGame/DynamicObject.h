#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"
#include "TerrainObject.h"

const float pi = 3.14159265358979323846f;

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };
enum Actions { bottomHit = 0, upperHit = 1, directHit = 2, relax = 3, combatState = 4, move = 5, dead = 6, evasionDown = 7, evasionUp = 8,commonHit = 9, hardHit = 10, specialHit = 11, transitionToEnotherWorld = 12, openInventory = 13, absorbs = 14, grab = 15, builds = 16 };
enum Side { up = 1, right = 2, down = 3, left = 4 };

class DynamicObject : public WorldObject
{
protected:	
	int ellipseSize;
	Vector2f movePosition = Vector2f(-1, -1);
	Vector2f focus1, focus2;
	float healthPoint, armor = 1, strength = 0, maxHealthPointValue;	
	float speed;
	float timeAfterHitself, timeForNewRoute, timeForNewSide = 100000, timeAfterSideChange;
	void setSide(Vector2i otherObjectPosition, float elapsedTime);
	Side side;	
	Actions currentAction, lastAction;
	Direction direction;
	Vector2f moveOffset = Vector2f (-1, -1);
	WorldObject *selectedTarget = nullptr;
public:
	DynamicObject(std::string objectName, Vector2f centerPosition);
	virtual ~DynamicObject();
	int getSpriteNumber() override { return currentSprite; }
	int getEllipseSize() const { return ellipseSize; }
	float getMaxHealthPointValue() { return maxHealthPointValue; }
	float getHealthPoint() { return healthPoint; }	
	float getSpeed() { return speed; }
	float getStrength() { return strength; }
	float getTimeAfterHitself() { return timeAfterHitself; }
	float getTimeForNewRoute() { return timeForNewRoute; }	
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getMoveOffset() { return moveOffset; }
	Vector2f getMovePosition() { return movePosition; }
	Actions getCurrentAction() { return currentAction; }
	Direction getDirection() { return direction; }
	WorldObject *getSelectedTarget() { return selectedTarget; }
	
	void setCurrentAction(Actions action) { this->currentAction = action; }
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	float setTimeAfterHitself(float time) { timeAfterHitself = time; }

	void increaseTimeForNewRoute(float value) { timeForNewRoute += value; }
	void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void takeDamage(float damage);			
	void setMoveOffset();
	void resetTimeForNewRoute() { timeForNewRoute = 0; }
	virtual void handleInput();
	virtual void behaviorWithDynamic(DynamicObject& target, float elapsedTime) = 0;
	virtual void behaviorWithStatic(WorldObject& target, float elapsedTime) = 0;
	virtual void behavior(float elapsedTime) = 0;
	virtual void setTarget(DynamicObject &object) = 0;

	float timeForNewHitself;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic);
	Vector2f EllipceSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, float elapsedTime);
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, float elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, float elapsedTime);
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route;
};

#endif