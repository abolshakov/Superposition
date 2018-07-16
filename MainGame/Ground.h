#pragma once
#ifndef GROUND_H
#define GROUND_H

#include "TerrainObject.h"

class Ground : public StaticObject
{
protected:
public:
	Ground(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
};

#endif