#include "PickedObject.h"



PickedObject::PickedObject(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	
}

bool PickedObject::pickUp(std::vector<std::pair <int, int>>& inventory)
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
				return true;
			}
			else
			{
				curInvItem->first = id;
				count -= maxCount - curInvItem->second;
				curInvItem->second = maxCount;
			}
		}
	}

	if (count <= 0)
	{
		delatePromiseOn();
		return true;
	}
	else
	{
		return false;
	}
}

PickedObject::~PickedObject()
{
}
