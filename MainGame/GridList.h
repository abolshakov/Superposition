#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>
#include <unordered_map>

template <class T>
class GridList
{	
	int width, height, size;
	std::vector<std::vector<T*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;
public:
	GridList();
	GridList(int width, int height, int size);
	~GridList();
	int getIndexByPoint(int x, int y) const;
	void addItem(T* item, const std::string& name, int x, int y);
	T* getItemByName(std::string& name);
	std::vector<T*> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
};

// ReSharper disable once CppUnusedIncludeDirective
#include "GridList.cpp"
#endif

