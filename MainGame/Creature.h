#pragma once
#ifndef CREATURE_H
#define CREATURE_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class Creature : public DynamicObject
{
protected:
	int sightRange;
	int morality;
	int fear;
private:
	float timeForNewSprite;
	int animationLength;
	bool wasHit = false;
	Clock timer;
	float timeAfterHit = 0, timeForNewHit = 100000;	
	int strikingSprite = 4;
public:
	Creature(std::string objectName, Vector2f centerPosition);
	~Creature();
	void behavior(DynamicObject& target, float elapsedTime) override;
};

#endif