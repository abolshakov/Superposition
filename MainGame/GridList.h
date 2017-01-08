#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>

template <class T>
class GridList
{	
	int width, height, size;
	std::vector<std::vector<T>> cells;
public:
	GridList();
	GridList(int width, int height, int size);
	int getIndexByPoint(int x, int y);
	void addItem(T item, int x, int y);
	std::vector<T> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
};

#include "GridList.cpp"
#endif

