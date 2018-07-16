#pragma once
#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include "StaticObject.h"

class TerrainObject : public StaticObject
{
protected:
	Vector2i focus1, focus2;
	int ellipseSize;
public:
	TerrainObject(std::string objectName, Vector2f centerPosition);
	~TerrainObject();
	float interactRadius, intersectsRadius;	
	Vector2i getFocus1() const { return focus1; }
	Vector2i getFocus2() const { return focus2; }
	int getEllipseSize() const { return ellipseSize; }
};

#endif