#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"
#include "TerrainObject.h"

const float pi = 3.14159265358979323846f;

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };
enum Actions { directHit = 2, relax = 3, combatState = 4, move = 5, dead = 6, commonHit = 9, moveHit = 10, dropping = 11, transitionToEnotherWorld = 12, 
	openInventory = 13, absorbs = 14, grab = 15, builds = 16, jerking = 17, upFlap = 30, leftFlap = 31, rightFlap = 32, startFlap = 33, stopFlap = 34, throwNoose = 35 };
enum Side { up = 1, right = 2, down = 3, left = 4 };

class DynamicObject : public WorldObject
{
protected:	
	int ellipseSize;
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float healthPoint, armor = 1, strength = 0, maxHealthPointValue;	
	float defaultSpeed, speed;
	float timeAfterHitself = 0, timeForNewRoute = 100000, timeAfterNewRoute = 0;
	void setSide(Vector2f otherObjectPosition, float elapsedTime);
	Side side;	
	Actions currentAction, lastAction = relax;
	Direction direction;
	Vector2f moveOffset = Vector2f (-1, -1);
	WorldObject *boundTarget = nullptr;
	bool routeGenerationAbility = true;
	DynamicItemsIdList id;

	//jerk mechanics
	float jerkPower, jerkDeceleration = 0, jerkDistance = 0;
	float jerkDuration = 1000000, jerkTime = 0;
	bool isJerking = false;
	//
public:
	DynamicObject(std::string objectName, Vector2f centerPosition);
	virtual ~DynamicObject();
	int getSpriteNumber() override { return currentSprite[0]; }
	int getEllipseSize() const { return ellipseSize; }
	float getMaxHealthPointValue() { return maxHealthPointValue; }
	float getHealthPoint() { return healthPoint; }	
	float getSpeed() { return speed; }
	float getStrength() { return strength; }
	float getTimeAfterHitself() { return timeAfterHitself; }
	float getTimeAfterNewRoute() { return timeAfterNewRoute; }
	float getTimeForNewRoute() { return timeForNewRoute; }
	bool getRouteGenerationAbility() { return routeGenerationAbility; }
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getMoveOffset() { return moveOffset; }
	Vector2f getMovePosition() { return movePosition; }
	Actions getCurrentAction() { return currentAction; }
	Direction getDirection() { return direction; }
	Side getSide() { return side; }
	WorldObject *getSelectedTarget() { return boundTarget; }
	DynamicItemsIdList getId() { return id; }
	static std::string sideToString(Side side);
	static std::string directionToString(Direction direction);

	void setCurrentAction(Actions action) { this->currentAction = action; }
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	float setTimeAfterHitself(float time) { timeAfterHitself = time; }

	void increaseTimeAfterNewRoute(float value) { timeAfterNewRoute += value; }
	void resetTimeAfterNewRoute() { timeAfterNewRoute = 0; }
	void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void takeDamage(float damage);			
	void setMoveOffset(float elapsedTime);	
	void changeAction(Actions newAction, bool resetSpriteNumber = false, bool rememberLastAction = false);
	virtual void handleInput();
	virtual void behaviorWithDynamic(DynamicObject* target, float elapsedTime) = 0;
	virtual void behaviorWithStatic(WorldObject* target, float elapsedTime) = 0;
	virtual void behavior(float elapsedTime) = 0;
	virtual void setTarget(DynamicObject &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destinationPoint) = 0;

	float timeForNewHitself;
	float timeAfterHit = 0, timeForNewHit = 100000;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic);
	bool canCrashIntoDynamic = true, canCrashIntoStatic = true;
	Vector2f EllipceSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, float elapsedTime);
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, float elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, float elapsedTime);
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route;

	std::string debugInfo;
	Vector2f memorizedRoutePosition = { -1, -1 };
};

#endif