#pragma once
#include "TerrainObject.h"

class RoseTree : public TerrainObject
{
protected:
public:
	RoseTree(std::string objectName, Vector2f centerPosition);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	const std::string getSpriteName(long long elapsedTime) override;
	int strength;
};
