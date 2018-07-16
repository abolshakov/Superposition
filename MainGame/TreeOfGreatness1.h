/*#pragma once
#include "TerrainObject.h"

class TreeOfGreatness : public TerrainObject
{
protected:
public:
	TreeOfGreatness(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	int getSpriteNumber() override { return 0; }
};*/
