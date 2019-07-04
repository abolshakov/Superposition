#pragma once
#ifndef HARE_H
#define HARE_H

#include "NeutralMob.h"
#include "Helper.h"

using namespace sf;

class Hare : public NeutralMob
{
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 10e5, timeAfterFear = 0, fearTime = 2 * 10e5;
	int strikingSprite = 4;
	WorldObject* owner = nullptr;
public:
	Hare(std::string objectName, Vector2f centerPosition);
	~Hare();
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime) override;
	void setTarget(DynamicObject& object) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
};

#endif