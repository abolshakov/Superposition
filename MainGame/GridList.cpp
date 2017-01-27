#pragma once
#ifndef GRIDLIST_CPP
#define GRIDLIST_CPP

#include <cmath>
// ReSharper disable once CppUnusedIncludeDirective
#include "GridList.h"

template <class T>
GridList<T>::GridList() : width(0), height(0), size(0)
{
}

template <class T>
GridList<T>::GridList(int width, int height, int size)
{
	this->width = width;
	this->height = height;
	this->size = size;
	auto vectorSize = int(ceil(double(height) / size) * ceil(double(width) / size));
	cells.resize(vectorSize);
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
}

template <class T>
int GridList<T>::getIndexByPoint(int x, int y) const
{
	auto y1 = y / size;
	auto x1 = ceil(double(width) / size);
	auto result = x1 * y1 + x / size;
	return int(result);
}

template <class T>
void GridList<T>::addItem(T* item, const std::string& name, int x, int y)
{
	if (items.find(name) != items.end())
		throw std::invalid_argument("The key '" + name + "' already exists in the Grid.");

	auto index = getIndexByPoint(x, y);
	auto position = std::make_pair(index, int(cells[index].size()));
	cells[index].push_back(item);	
	items.insert({ name, position });
}

template <class T>
T* GridList<T>::getItemByName(std::string& name)
{
	auto position = items.at(name);
	return cells[position.first][position.second];
}

template <class T>
std::vector<T*> GridList<T>::getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	if (upperLeftX < 0)
		upperLeftX = 0;
	if (upperLeftY < 0)
		upperLeftY = 0;
	if (bottomRightX > width)
		bottomRightX = width;
	if (bottomRightY > height)
		bottomRightY = height;

	std::vector<T*> result;
	for (auto i = 0; i <= ceil(double(bottomRightY)/size) - ceil(double(upperLeftY) / size); i++)
	{
		int border = upperLeftY + i*size;
		if (border > height)
			border = height;
		for (auto j = getIndexByPoint(upperLeftX, border); j <= getIndexByPoint(bottomRightX, border); j++)
		{
			for (auto k = 0; k < cells[j].size(); k++)
			{
				result.push_back(cells[j][k]);
			}
		}
	}
	return result;
}

template <class T>
void GridList<T>::updateItemPosition(const std::string& name, int x, int y)
{
	auto position = items.at(name);	
	auto item = cells[position.first][position.second];
	cells[position.first].erase(cells[position.first].begin()+position.second);

	auto index = getIndexByPoint(x, y);
	position = std::make_pair(index, int(cells[index].size()));
	cells[index].push_back(item);
	items[name] = position;

}

#endif
