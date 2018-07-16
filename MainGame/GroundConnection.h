#pragma once
#ifndef GROUNDCONNECTION_H
#define GROUNDCONNECTION_H

#include "TerrainObject.h"

class GroundConnection : public StaticObject
{
protected:
public:
	GroundConnection(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
};

#endif