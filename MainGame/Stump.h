#pragma once
#ifndef STUMP_H
#define STUMP_H

#include "TerrainObject.h"

class Stump : public TerrainObject
{
protected:
public:
	Stump(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif