#pragma once
#include "TerrainObject.h"

class RoseTree : public TerrainObject
{
protected:
public:
	RoseTree(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
};
