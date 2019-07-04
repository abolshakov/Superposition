#pragma once
#ifndef FOG_H
#define FOG_H

#include "TerrainObject.h"

class Fog : public TerrainObject
{
protected:
public:
	Fog(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setType(int typeOfObject) override;
};

#endif