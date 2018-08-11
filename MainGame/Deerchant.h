#pragma once
#ifndef DEERCHANT_H
#define DEERCHANT_H

#include "DynamicObject.h"
#include "World.h"

using namespace sf;

class Deerchant : public DynamicObject
{
private:
	void setHitDirection();
	float energy, maxEnergyValue, energyForSpecial;	
	WorldObject *selectedTarget;
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
	std::string getSpriteName(long long elapsedTime) override;
	//control
	void handleInput() override;	
	WorldObject *getSelectedTarget() { return selectedTarget; }
	void behaviorWithDynamic(DynamicObject& target, float elapsedTime) override;
	void behaviorWithStatic(WorldObject& target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
	void onMouseDownBehavior(WorldObject *object);
};

#endif