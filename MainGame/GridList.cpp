#pragma once

#ifndef GRIDLIST_CPP
#define GRIDLIST_CPP

#include <cmath>
// ReSharper disable once CppUnusedIncludeDirective
#include "GridList.h"
#include <iostream>

template <class T>
GridList<T>::GridList() : width(0), height(0), size(0, 0)
{
}

template <class T>
Vector2i GridList<T>::getBlockSize() const
{
	return size;
}

template <class T>
GridList<T>::GridList(int width, int height, Vector2i size)
{
	this->width = width;
	this->height = height;
	this->size = size;
	auto vectorSize = int(ceil(double(height) / size.y) * ceil(double(width) / size.x));
	cells.resize(vectorSize);
	for (auto arr : cells)
	{
		arr.resize(0);
	}
}

template <class T>
GridList<T>::~GridList()
{
	for (std::vector<T*> cell : cells)
	{
		for (T* ptr : cell)
		{
			delete ptr;
		}
		cell.clear();
	}
	cells.clear();
	items.clear();
}

template <class T>
int GridList<T>::getIndexByPoint(int x, int y) const
{
	auto y1 = y / size.y;
	auto x1 = ceil(double(width) / size.x);
	auto result = x1 * y1 + x / size.x;
	return int(result);
}

template <class T>
Vector2f GridList<T>::getPointByIndex(int index) const
{
	int inLineNumber = ceil(double(width) / size.x);
	int inRawNumber = (index) / inLineNumber;

	Vector2f result;
	result.x = (index % inLineNumber) * size.x;
	result.y = (inRawNumber * size.y);

	return result;
}

template <class T>
void GridList<T>::addItem(T* item, const std::string& name, int x, int y)
{
	int blocksCount = ceil(width / size.x) * ceil(height / size.y);

	if (items.find(name) != items.end())
		throw std::invalid_argument("The key " + name + " already exists in the Grid.");

	auto index = getIndexByPoint(x, y);	

	auto position = std::make_pair(index, int(cells[index].size()));
	cells[index].push_back(item);
	items.insert({ name, position });
}

template <class T>
void GridList<T>::clearCell(int cellIndex)
{
	for (auto item : cells[cellIndex])
	{
		auto itemObject = dynamic_cast<WorldObject*>(item);
		if (itemObject)
			items.erase(items.find(itemObject->getName()));
		itemObject->~WorldObject();
	}
	cells[cellIndex].clear();
}

template <class T>
void GridList<T>::deleteItem(std::string name)
{	
	auto position = items.at(name);
	for (int i = position.second; i < cells[position.first].size(); i++)
	{
		auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	auto itemObject = dynamic_cast<WorldObject*>(cells[position.first][position.second]);
	itemObject->~WorldObject();
	cells[position.first].erase(cells[position.first].begin() + position.second);
	items.erase(items.find(name));
}

template <class T>
T* GridList<T>::getItemByName(const std::string& name)
{
	auto position = items.at(name);
	return cells[position.first][position.second];
}

template <class T>
std::vector<T*> GridList<T>::getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	std::vector<T*> result;
	if (upperLeftX <= 0)
	{
		upperLeftX = 0;
	}
	if (bottomRightX >= width)
	{
		bottomRightX = width;
	}
	if (upperLeftY <= 0)
	{
		upperLeftY = 0;
	}
	if (bottomRightY >= height)
	{
		bottomRightY = height;
	}
	auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	auto columnsPerRow = int(ceil(double(width) / size.x));
	auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (int j = firstColumn; j <= lastColumn; j++)
		{
			result.insert(result.end(), cells[j].begin(), cells[j].end());
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
		
	}
	return result;
}

template <class T>
std::vector<T*> GridList<T>::getItems(int blockIndex)
{
	return cells[blockIndex];
}

template <class T>
std::vector<int> GridList<T>::getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	std::vector<int> result;

	if (upperLeftX <= 0)
		upperLeftX = 0;
	if (bottomRightX >= width)
		bottomRightX = width;
	if (upperLeftY <= 0)
		upperLeftY = 0;
	if (bottomRightY >= height)
		bottomRightY = height;

	auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	auto columnsPerRow = int(ceil(double(width) / size.x));
	auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		if (i == 0 || i == rowsCount)
			for (int j = firstColumn; j <= lastColumn; j++)
				result.push_back(j);
		else
		{
			result.push_back(firstColumn);
			result.push_back(lastColumn);
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

template <class T>
void GridList<T>::updateItemPosition(const std::string name, int x, int y)
{
	auto position = items.at(name);
	auto item = cells[position.first][position.second];
	cells[position.first].erase(cells[position.first].begin() + position.second);

	for (int i = position.second; i < cells[position.first].size(); i++)
	{
		auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	auto index = getIndexByPoint(x, y);
	position = std::make_pair(index, int(cells[index].size()));
	items.at(name) = position;
	cells[index].push_back(item);
}

#endif
