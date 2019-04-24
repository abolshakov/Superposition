#pragma once
#ifndef DEERCHANT_H
#define DEERCHANT_H

#include "DynamicObject.h"
#include "HeroBag.h"
#include "World.h"

using namespace sf;

class Deerchant : public DynamicObject
{
private:
	void setHitDirection();
	float energy, maxEnergyValue, energyForSpecial;
	WorldObject* owner = nullptr;
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
	void prepareSpriteNames(long long elapsedTime) override;
	//control
	void handleInput() override;		
	void behaviorWithDynamic(DynamicObject* target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	void onMouseDownBehavior(WorldObject *object, Vector2f mouseWorldPos, bool isBuilding = false);
	void setTarget(DynamicObject& object) override;
	void endingPreviousAction();
	void stopping(bool doStand = false, bool forgetSelectedTarget = false);
	Vector2f getBeltPosition();
	//jerk
	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void jerkInteract(float elapsedTime);

	bagCell* heldItem = nullptr;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	std::vector<HeroBag> bags;
};

#endif