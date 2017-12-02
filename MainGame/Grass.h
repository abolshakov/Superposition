#pragma once
#include "TerrainObject.h"

class Grass : public TerrainObject
{
protected:
public:
	Grass(std::string objectName, Vector2f centerPosition);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	Vector2f getScaleRatio();
};
