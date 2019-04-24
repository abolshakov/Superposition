#pragma once
#ifndef PICKEDOBJECT_H
#define PICKEDOBJECT_H

#include "TerrainObject.h"
#include "HeroBag.h"

class PickedObject : public TerrainObject
{
protected:
	lootItemsIdList id;
	int count;
public:
	PickedObject::PickedObject(std::string objectName, Vector2f centerPosition);
	virtual ~PickedObject();
	bool pickUp(std::vector<HeroBag> *bags);
	lootItemsIdList getId() { return id; }
	int getCount() { return count; }
};

#endif