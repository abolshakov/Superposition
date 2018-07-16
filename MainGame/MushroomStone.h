#pragma once
#ifndef MUSHROOMSTONE_H
#define MUSHROOMSTONE_H

#include "TerrainObject.h"

class MushroomStone : public TerrainObject
{
protected:
public:
	MushroomStone(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif