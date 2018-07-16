#pragma once
#ifndef HOMECOSINESS_H
#define HOMECOSINESS_H

#include "TerrainObject.h"

class HomeCosiness : public TerrainObject
{
protected:
public:
	HomeCosiness(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject) override;
};

#endif