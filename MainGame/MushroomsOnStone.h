#pragma once
#ifndef MUSHROOMSONSTONE_H
#define MUSHROOMSONSTONE_H

#include "TerrainObject.h"

class MushroomsOnStone : public TerrainObject
{
protected:
public:
	MushroomsOnStone(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif