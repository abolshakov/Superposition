#ifndef MONSTER_H
#define MONSTER_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

//enum Actions {bottomHit = 0, upperHit = 1, directHit = 2, relax = 3, combatState = 4, move = 5};

class Monster : public DynamicObject
{
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	
	int strikingSprite = 4;
public:
	Monster(std::string objectName, Vector2f centerPosition);
	~Monster();
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime) override;
	void behaviorWithDynamic(DynamicObject* target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
};

#endif