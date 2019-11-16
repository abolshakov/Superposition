#ifndef MONSTER_H
#define MONSTER_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class Monster : public DynamicObject
{
public:
	Monster(std::string objectName, Vector2f centerPosition);
	~Monster();
	void behaviorWithDynamic(DynamicObject* target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	virtual void endingPreviousAction();
	void stopping(bool doStand, bool forgetSelectedTarget);
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	WorldObject* getVictim() const { return victim; }
	void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fightLogic(float elapsedTime, DynamicObject* target = nullptr) override;
	virtual void doAttack(WorldObject* target) = 0;
protected:
	WorldObject* victim = nullptr;
	std::map<Actions, bool> isAttack = { {commonHit, true}, {directHit, true} };
	float hitDistance = 0, sightRange = 0;
	int animationLength;
	bool wasHit = false;

	int strikingSprite = 0;
};
#endif