#pragma once
#ifndef _GRIDLIST_CPP_
#define _GRIDLIST_CPP_

#include "GridList.h"
#include <cmath>

template <class T>
GridList<T>::GridList()
{
}

template <class T>
GridList<T>::GridList(int width, int height, int size)
{
	this->width = width;
	this->height = height;
	this->size = size;
	int vectorSize = ceil(double(height) / size)*ceil(double(width) / size);
	cells.resize(vectorSize);
}

template <class T>
int GridList<T>::getIndexByPoint(int x, int y)
{
	int y1 = y / size;
	int x1 = ceil(double(width) / size);
	int res = x1*y1;
	res += x / size;
	return res;
}

template <class T>
void GridList<T>::addItem(T item, int x, int y)
{
	cells[getIndexByPoint(x, y)].push_back(item);
}

template <class T>
std::vector<T> GridList<T>::getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	if (upperLeftX < 0)
		upperLeftX = 0;
	if (upperLeftY < 0)
		upperLeftY = 0;

	if (bottomRightX > width)
		upperLeftX = width;
	if (bottomRightY > height)
		bottomRightY = height;
	std::vector<T> result;
	for (int i = 0; i < ceil(double(bottomRightY - upperLeftY) / size); i++)
	{
		for (int j = getIndexByPoint(upperLeftX, upperLeftY + i*size); j < getIndexByPoint(bottomRightX, upperLeftY + i*size); j++)
		{
			for (int k = 0; k < cells[j].size(); k++)
			{
				result.push_back(cells[j][k]);
			}
		}
	}
	return result;
}

#endif
