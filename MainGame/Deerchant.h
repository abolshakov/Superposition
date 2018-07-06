#pragma once
#ifndef DEERCHANT_H
#define DEERCHANT_H

#include "DynamicObject.h"
#include "World.h"

using namespace sf;

class Deerchant : public DynamicObject
{
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
	//draw
	std::string nameOfFile;
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	//control
	void handleInput() override;	

	void behavior(DynamicObject& target, float elapsedTime) override;	
};

#endif