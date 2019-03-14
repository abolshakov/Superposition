#pragma once
#ifndef GRASS_H
#define GRASS_H

#include "TerrainObject.h"

class Grass : public StaticObject
{
protected:
public:
	Grass(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif