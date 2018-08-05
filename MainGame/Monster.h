#ifndef MONSTER_H
#define MONSTER_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

//enum Actions {bottomHit = 0, upperHit = 1, directHit = 2, relax = 3, combatState = 4, move = 5};

class Monster : public DynamicObject
{
private:
	float timeForNewSprite;

	int animationLength;
	bool wasHit = false;
	Clock timer;
	float timeAfterHit = 0, timeForNewHit = 100000;
	int strikingSprite = 4;
public:
	Monster(std::string objectName, Vector2f centerPosition);
	~Monster();
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	void behavior(DynamicObject& target, float elapsedTime) override;
};

#endif