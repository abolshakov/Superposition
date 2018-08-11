#pragma once
#ifndef HARETRAP_H
#define HARETRAP_H

#include "TerrainObject.h"

class HareTrap : public TerrainObject
{
protected:
public:
	HareTrap(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif