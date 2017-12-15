#pragma once
#include "DynamicObject.h"

using namespace sf;

//enum Actions {bottomHit = 0, upperHit = 1, directHit = 2, relax = 3, combatState = 4, move = 5};

class Enemy : public DynamicObject
{
	float timeForNewSprite;
	
	int animationLength;
	bool wasHit = false;
	Clock timer;
	float timeAfterHit = 0, timeForNewHit = 100000;		
	int strikingSprite = 4;
public:			
	//!
	HitDirection hitDirection;
	Actions currentAction, lastAction;
	int currentSprite;
	Vector2f targetPosition;
	//!

	std::string nameOfFile;
	Enemy(std::string objectName, Vector2f centerPosition);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	void behavior(DynamicObject& target);
	void moveToTarget(Vector2f ptargetPosition, float targetRadius);
};

