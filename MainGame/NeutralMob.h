#pragma once
#ifndef NEUTRALMOB_H
#define NEUTRALMOB_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class NeutralMob : public DynamicObject
{
protected:
	int sightRange;
	int morality;
	int fear;
private:
public:
	NeutralMob(std::string objectName, Vector2f centerPosition);
	~NeutralMob();
	void behaviorWithDynamic(DynamicObject& target, float elapsedTime) override;
	virtual void behaviorWithStatic(WorldObject& target, float elapsedTime) override = 0;
	void behavior(float elapsedTime) override = 0;
	void setTarget(DynamicObject& object) override;
};

#endif