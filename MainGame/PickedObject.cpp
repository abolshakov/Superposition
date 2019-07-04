#include "PickedObject.h"



PickedObject::PickedObject(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	
}

bool PickedObject::pickUp(std::vector<HeroBag> *bags)
{
	if (this->tag == heroBagTag)
		return true;
	while (true)
	{
		bagCell *maximumFilledCell = nullptr;

		for (auto& bag : *bags)
		{
			for (auto& cell : bag.cells)
			{
				if (maximumFilledCell == nullptr)
				{
					if (cell.content.first == lootItemsIdList::bagCell || lootItemsIdList(cell.content.first) == id && cell.content.second < HeroBag::itemsMaxCount.at(cell.content.first))
						maximumFilledCell = &cell;
				}
				else
					if (lootItemsIdList(cell.content.first) == id && cell.content.second > maximumFilledCell->content.second && cell.content.second < HeroBag::itemsMaxCount.at(cell.content.first))
						maximumFilledCell = &cell;					
			}
		}
		if (maximumFilledCell != nullptr)
		{
			maximumFilledCell->content.first = id;
			maximumFilledCell->content.second += count;
			if (maximumFilledCell->content.second > HeroBag::itemsMaxCount.at(lootItemsIdList(id)))
			{
				count = maximumFilledCell->content.second % HeroBag::itemsMaxCount.at(lootItemsIdList(id));
				maximumFilledCell->content.second = HeroBag::itemsMaxCount.at(lootItemsIdList(id));
			}
			else
			{
				count = 0;
				id = lootItemsIdList::bagCell;
				deletePromiseOn();
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
