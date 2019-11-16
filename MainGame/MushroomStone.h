#pragma once
#ifndef MUSHROOMSTONE_H
#define MUSHROOMSTONE_H

#include "TerrainObject.h"

class MushroomStone : public TerrainObject
{
protected:
public:
	MushroomStone(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif