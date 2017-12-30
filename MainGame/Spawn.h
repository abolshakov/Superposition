#pragma once
#include "TerrainObject.h"

class Spawn : public TerrainObject
{
protected:
public:
	Spawn(std::string objectName, Vector2f centerPosition);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	int getSpriteNumber() override { return 0; }
};
