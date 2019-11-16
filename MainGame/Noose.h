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
	void rotateAndExtend(spriteChainElement* rope, spriteChainElement* loop);

	float timeForNewSprite, timeAfterHit = 0, timeForNewHit = 100000;
	float ropeElongation = 0, maximumLength = 1500;
	int animationLength, strikingSprite = 4;
	bool wasHit = false;
	spriteChainElement stillRope, stillLoop;
	WorldObject* owner;
	Vector2f ownerPos = { 0, 0 };
	Vector2i ownerGlobalBounds = { 0, 0 };
public:
	Noose(std::string objectName, Vector2f centerPosition, WorldObject* owner);
	~Noose();
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
    void behaviorWithDynamic(DynamicObject* target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;	
	void setOwner(WorldObject* owner) { this->owner = owner; };
	void setTarget(DynamicObject& object) override;
	void jerkInteract(float elapsedTime);
	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void fightLogic(float elapsedTime, DynamicObject* target = nullptr) override;
};

#endif