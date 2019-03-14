#pragma once
#ifndef HOMECOSINESS_H
#define HOMECOSINESS_H

#include "TerrainObject.h"

class HomeCosiness : public TerrainObject
{
protected:
public:
	HomeCosiness(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif