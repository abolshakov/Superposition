#pragma once
#ifndef NOOSE_H
#define NOOSE_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class Noose : public DynamicObject
{
private:
    void stopping(bool doStand = false, bool forgetSelectedTarget = false);
    void endingPreviousAction();

	float timeForNewSprite;
	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
public:
	Noose(std::string objectName, Vector2f centerPosition);
	~Noose();
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime) override;
    void behaviorWithDynamic(DynamicObject& target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject& target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
};

#endif