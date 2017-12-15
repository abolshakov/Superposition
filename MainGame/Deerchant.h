#pragma once
#include "DynamicObject.h"
#include "World.h"

using namespace sf;

class Deerchant : public DynamicObject
{
	float timeForNewSprite;
	int currentSprite;
	int animationLength;	
	void setHitDirection();
	float energy, maxEnergyValue, energyForSpecial;	
public:	
	Deerchant(std::string objectName, Vector2f centerPosition);
	//builder
	bool isBuilder = false;
	//fight
	bool isFightWithBoss = false;
	float getEnergy() { return energy;  }
	float getMaxEnergyValue() { return maxEnergyValue; }
	float getEnergyForSpecial() { return energyForSpecial; }
	void setEnergy(float energy) { this->energy = energy; }
	void addEnergy(float energy) { this->energy += energy; if (this->energy > this->maxEnergyValue) this->energy = this->maxEnergyValue; }
	HitDirection hitDirection;
	void doKick(std::vector<DynamicObject>& dynamicItems, Vector2i mousePos, float scaleFactor);
	//draw
	int getCurrentSprite() { return currentSprite; }
	std::string nameOfFile;
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	//control
	void handleInput() override;		
};

