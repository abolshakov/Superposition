#pragma once
#ifndef GRASS_H
#define GRASS_H

#include "TerrainObject.h"

class Grass : public StaticObject
{
protected:
public:
	Grass(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
};

#endif