#pragma once
#ifndef CLAPWHILR_H
#define CLAPWHILR_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class ClapWhirl : public DynamicObject
{
public:
	ClapWhirl(std::string objectName, Vector2f centerPosition, WorldObject* owner);
	~ClapWhirl();
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
private:
	void stopping(bool doStand = false, bool forgetSelectedTarget = false);
	void endingPreviousAction();
	void rotateAndExtend(spriteChainElement* whirl);

	float timeForNewSprite, timeAfterHit = 0, timeForNewHit = 100000;
	int animationLength, strikingSprite = 4;
	std::map<Tag, bool> cantStopMe = { {Tag::chamomile, true},  {Tag::yarrow, true}, {Tag::mugwort, true}, {Tag::poppy, true}, {Tag::fern, true}, {Tag::stump, true}, {Tag::rock, true} };
	bool wasHit = false;
	spriteChainElement stillRope, stillLoop;
	WorldObject* owner;
	Vector2f ownerPos = { 0, 0 }, startPos = { 0, 0 };
	Vector2i ownerGlobalBounds = { 0, 0 };
};

#endif