#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

using namespace sf;

template <class T>
class GridList
{
	int width, height;
	Vector2i size;
	std::vector<std::vector<T*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;
public:
	GridList();
	GridList(int width, int height, Vector2i size);
	~GridList();
	Vector2i getBlockSize() const;
	int getIndexByPoint(int x, int y) const;
	void addItem(T* item, const std::string& name, int x, int y);
	T* getItemByName(const std::string& name);
	std::vector<T*> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int width);
	void updateItemPosition(const std::string& name, int x, int y);
};

// ReSharper disable once CppUnusedIncludeDirective
#include "GridList.cpp"
#endif

