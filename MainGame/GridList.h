#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "WorldObject.h"

using namespace sf;

const int inf = 10e6;

template <class T>
class GridList
{
private:
	int width, height;
	Vector2i size, microsize;
	std::vector<std::vector<T*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;
	void GridList<T>::dfs(int i, int j, int iBorder, int jBorder, int startX, int startY, int finishX, int finishY);
	void GridList<T>::bfs(int iBorder, int jBorder, int startX, int startY, int finishX, int finishY);
public:
	std::unordered_map<std::string, std::pair<int, int>> getItemsOfGreed() { return items; }
	GridList();
	GridList(int width, int height, Vector2i size, Vector2i microsize);
	~GridList();
	Vector2i getBlockSize() const;
	int getBlockItemsAmount(int index) { return cells[index].size(); }
	int getIndexByPoint(int x, int y) const;
	int getMicroblockByPoint(int x, int y) const;
	Vector2f getPointByMicroblock(int microblockIndex) const;
	Vector2f getPointByIndex(int index) const;
	void fillLocalMatrix(Vector2f targetPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	void makeRoute(Vector2f startPos, Vector2f finishPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	void addItem(T* item, const std::string& name, int x, int y);
	T* getItemByName(const std::string& name);
	std::vector<T*> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	std::vector<T*> getItems(int blockIndex);
	void updateItemPosition(const std::string name, int x, int y);
	std::vector<std::vector<T*>> getCells(){ return cells; }
	void clearCell(int cellIndex);
	void deleteItem(std::string name);
	int getSize() { return items.size(); }
	std::vector<int> getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int offset);
	std::vector<int> GridList<T>::getBlocksInSight(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	bool isIntersectWithOthers(Vector2f position1, float radius1, std::vector<WorldObject*> visibleTerrain, bool isDotAdjustded = false) const;

	std::vector<std::vector<int>> microblockMatrix;
	std::vector<std::vector<float>> distances;
	std::vector<std::vector<bool>> visited;
	std::vector<std::vector<std::vector<std::pair<int, int>>>> routes;
	std::pair<int, int> reserveDestination = std::make_pair(-1, -1);
};

#include "GridList.cpp"
#endif

