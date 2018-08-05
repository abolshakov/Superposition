#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "WorldObject.h"

using namespace sf;

template <class T>
class GridList
{
private:
	int width, height;
	Vector2i size;
	std::vector<std::vector<T*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;
public:
	std::unordered_map<std::string, std::pair<int, int>> getItemsOfGreed() { return items; }
	GridList();
	GridList(int width, int height, Vector2i size);
	~GridList();
	Vector2i getBlockSize() const;
	int getBlockItemsAmount(int index) { return cells[index].size(); }
	int getIndexByPoint(int x, int y) const;
	Vector2f getPointByIndex(int index) const;
	void addItem(T* item, const std::string& name, int x, int y);
	T* getItemByName(const std::string& name);
	std::vector<T*> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	std::vector<T*> getItems(int blockIndex);
	void updateItemPosition(const std::string name, int x, int y);
	std::vector<std::vector<T*>> getCells(){ return cells; }
	void clearCell(int cellIndex);
	void deleteItem(std::string name);
	int getSize() { return items.size(); }
	std::vector<int> getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
};

// ReSharper disable once CppUnusedIncludeDirective
#include "GridList.cpp"
#endif

