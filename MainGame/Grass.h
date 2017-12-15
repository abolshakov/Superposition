#pragma once
#include "TerrainObject.h"

class Grass : public StaticObject
{
protected:
public:
	Grass(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
};
