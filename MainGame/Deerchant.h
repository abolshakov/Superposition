#pragma once
#ifndef DEERCHANT_H
#define DEERCHANT_H

#include "DynamicObject.h"
#include "HeroBag.h"
#include "World.h"

using namespace sf;

class Deerchant : public DynamicObject
{
public:	
	Deerchant(std::string objectName, Vector2f centerPosition);
	~Deerchant();
	//BuildSystemMaker
	bool isBuildSystemMaker = false;
	//fight
	bool isFightWithBoss = false;
	float getEnergy() { return energy;  }
	float getMaxEnergyValue() { return maxEnergyValue; }
	float getEnergyForSpecial() { return energyForSpecial; }
	void setEnergy(float energy) { this->energy = energy; }
	void addEnergy(float energy) { this->energy += energy; if (this->energy > this->maxEnergyValue) this->energy = this->maxEnergyValue; }
	//draw
	std::string nameOfFile;
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	//control
	void handleInput(bool usedMouse = false) override;
	void behaviorWithDynamic(DynamicObject* target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	void onMouseUp(int currentMouseButton, WorldObject *mouseSelectedObject, Vector2f mouseWorldPos, bool isBuilding = false);
	void setTarget(DynamicObject& object) override;
	void endingPreviousAction();
	void stopping(bool doStand = false, bool forgetSelectedTarget = false, bool offUnsealInventory = false);
	void changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction) override;
	Vector2f getBeltPosition();
	//jerk
	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void jerkInteract(float elapsedTime);
	void fightLogic(float elapsedTime, DynamicObject* target = nullptr) override;

	Cell* heldItem = nullptr;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	std::vector<HeroBag> bags;
	bool nearTheTable = false;
private:
	void setHitDirection();

	float energy = 0, maxEnergyValue = 0, energyForSpecial = 0, hitDistance = 0;
	WorldObject* owner = nullptr, *unsealInventoryOwner = nullptr;
	int strikingSprite = 0;
};

#endif