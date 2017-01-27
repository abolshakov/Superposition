#pragma once
#include "StaticObject.h"

class TerrainObject : public StaticObject
{
protected:
	Vector2f focus1, focus2;
	int ellipseSize;
public:
	TerrainObject(std::string objectName, Vector2f centerPosition);
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	int getEllipseSize() const { return ellipseSize; }
};

