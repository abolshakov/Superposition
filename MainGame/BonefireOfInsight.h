#pragma once
#ifndef BONEFIREOFINSIGHT_H
#define BONEFIREOFINSIGHT_H

#include "TerrainObject.h"

class BonefireOfInsight : public TerrainObject
{
protected:
public:
	BonefireOfInsight(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setType(int typeOfObject);
};

#endif