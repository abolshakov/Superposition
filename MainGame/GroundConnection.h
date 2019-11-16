#pragma once
#ifndef GROUNDCONNECTION_H
#define GROUNDCONNECTION_H

#include "TerrainObject.h"

class GroundConnection : public StaticObject
{
protected:
public:
	GroundConnection(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif