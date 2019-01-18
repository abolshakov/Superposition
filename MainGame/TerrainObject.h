#pragma once
#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include "StaticObject.h"

class TerrainObject : public StaticObject
{
protected:
	Vector2f focus1, focus2;
	Vector2f dot1, dot2, currentDot = Vector2f (-1, -1);
	float ellipseSize;
public:
	TerrainObject(std::string objectName, Vector2f centerPosition);
	virtual ~TerrainObject();
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getDot1() const { return dot1; }
	Vector2f getDot2() const { return dot2; }
	Vector2f getCurrentDot() { return currentDot; }
	int getEllipseSize() const { return ellipseSize; }
	bool isIntersected(Vector2f curPosition, float radius, Vector2f newPosition); //const;
	std::vector<int> getMultiellipseIntersect(Vector2f position) const;
	Vector2f newSlippingPositionForDotsAdjusted(Vector2f position, Vector2f motionVector, float speed, float elapsedTime);

	std::vector<std::pair<std::pair<float, Vector2f>, std::pair<Vector2f, Vector2f>>> internalEllipses;
};

#endif