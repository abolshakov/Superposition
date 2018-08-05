#include "PickedObject.h"



PickedObject::PickedObject(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	
}

void PickedObject::pickUp(std::vector<std::pair <int, int>>& inventory)
{
	for (auto curInvItem = inventory.begin(); curInvItem != inventory.end(); ++curInvItem)
	{
		if (curInvItem->first == id || curInvItem->first == 0)
		{
			if (curInvItem->second + count <= maxCount)
			{
				curInvItem->first = id;
				curInvItem->second += count;
				//currentInventory.erase(currentInventory.begin() + currentCell);
				count = 0;
				delatePromiseOn();
				return;
			}
			else
			{
				curInvItem->first = id;
				count -= maxCount - curInvItem->second;
				curInvItem->second = maxCount;
			}
		}
	}
}

PickedObject::~PickedObject()
{
}
