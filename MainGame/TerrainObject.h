#pragma once
#include "StaticObject.h"

class TerrainObject : public StaticObject
{
protected:
	Vector2f focus1, focus2, ellipseCenterPosition;
	int ellipseSize;
public:
	TerrainObject(std::string objectName, Vector2f ellipseCenterPosition);
	Vector2f getellipsePosition() const { return Vector2f(ellipseCenterPosition.x, ellipseCenterPosition.y); }
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	int getellipseSize() const { return ellipseSize; }
};

