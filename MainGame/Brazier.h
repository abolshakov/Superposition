#pragma once
#ifndef BRAZIER_H
#define BRAZIER_H

#include "TerrainObject.h"

class Brazier : public TerrainObject
{
protected:
public:
	Brazier(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif