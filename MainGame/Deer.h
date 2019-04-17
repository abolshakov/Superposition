#pragma once
#ifndef DEER_H
#define DEER_H

#include "NeutralMob.h"
#include "Helper.h"

using namespace sf;

class Deer : public NeutralMob
{
protected:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
	WorldObject* owner = nullptr;
public:
	Deer(std::string objectName, Vector2f centerPosition);
	~Deer();
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	void endingPreviousAction();
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getHeadPosition();
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	WorldObject* getOwner() const { return owner; }
	//void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void setOwner(WorldObject* owner) { this->owner = owner; }
};

#endif