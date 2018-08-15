#pragma once
#ifndef HARE_H
#define HARE_H

#include "Creature.h"
#include "Helper.h"

using namespace sf;

class Hare : public Creature
{
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	Clock timer;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
public:
	Hare(std::string objectName, Vector2f centerPosition);
	~Hare();
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	void behaviorWithStatic(WorldObject& target, float elapsedTime) override;
	void behavior(float elapsedTime) override;
};

#endif