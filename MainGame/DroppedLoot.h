#pragma once
#ifndef DROPPEDLOOT_H
#define DROPPEDLOOT_H

#include "PickedObject.h"

class DroppedLoot : public PickedObject
{
protected:

public:
	DroppedLoot(std::string objectName, Vector2f centerPosition, int typeOfObject, int count = 1);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime) override;
	int strength{};
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;	
};

#endif