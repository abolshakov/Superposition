#pragma once
#ifndef HARETRAP_H
#define HARETRAP_H

#include "TerrainObject.h"

class HareTrap : public TerrainObject
{
protected:
public:
	HareTrap(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif