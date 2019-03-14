#include "PickedObject.h"



PickedObject::PickedObject(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	
}

bool PickedObject::pickUp(std::vector<HeroBag> *bags)
{
	while (true)
	{
		bagCell *maximumFilledCell = nullptr;

		for (auto& bag : *bags)
		{
			for (auto& cell : bag.cells)
			{
				if (maximumFilledCell == nullptr)
				{
					if (cell.content.first == -1 || cell.content.first == id && cell.content.second < maxCount)
						maximumFilledCell = &cell;
				}
				else
					if (cell.content.first == id && cell.content.second > maximumFilledCell->content.second && cell.content.second < maxCount)			
						maximumFilledCell = &cell;					
			}
		}
		if (maximumFilledCell != nullptr)
		{
			maximumFilledCell->content.first = id;
			maximumFilledCell->content.second += count;
			if (maximumFilledCell->content.second > maxCount)
				count = maximumFilledCell->content.second % maxCount;
			else
			{
				count = 0;
				id = -1;
				delatePromiseOn();
				return true;
			}
		}
		else
			return false;
	}
}

PickedObject::~PickedObject()
{
}
