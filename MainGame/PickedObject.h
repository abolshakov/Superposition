#pragma once
#ifndef PICKEDOBJECT_H
#define PICKEDOBJECT_H

#include "TerrainObject.h"

class PickedObject : public TerrainObject
{
protected:
	int id;
	int count, maxCount;
public:
	PickedObject::PickedObject(std::string objectName, Vector2f centerPosition);
	~PickedObject();
	bool pickUp(std::vector<std::pair <int, int>>& inventory);
	int getId() { return id; }
	int getCount() { return count; }
};

#endif