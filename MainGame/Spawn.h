#pragma once
#ifndef SPAWN_H
#define SPAWN_H

#include "TerrainObject.h"

class Spawn : public TerrainObject
{
protected:
public:
	Spawn(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif