#pragma once
#ifndef GRIDLIST_CPP
#define GRIDLIST_CPP

#include <cmath>
#include <queue>
#include "GridList.h"
#include "Helper.h"

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
GridList<T>::GridList(int width, int height, Vector2i size, Vector2i microsize)
{
	this->width = width;
	this->height = height;
	this->size = size;
	this->microsize = microsize;
	auto vectorSize = int(ceil(double(height) / size.y) * ceil(double(width) / size.x));
	cells.resize(vectorSize);
	for (auto&arr : cells)
	{
		arr.resize(0);
	}
	microblockMatrix.resize(500, std::vector<int>(500, 1));
	distances.resize(500, std::vector<float>(500, 55));
	visited.resize(500, std::vector<bool>(500, false));
	routes.resize(500, std::vector<std::vector<std::pair<int, int>>>(500, {{-1, -1}}));
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
int GridList<T>::getMicroblockByPoint(int x, int y) const
{
	auto y1 = y / microsize.y;
	auto x1 = ceil(double(width) / microsize.x);
	auto result = x1 * y1 + x / microsize.x;
	return int(result);
}

template <class T>
Vector2f GridList<T>::getPointByMicroblock(int microblockIndex) const
{
	int inLineNumber = ceil(double(width) / microsize.x);
	int inRawNumber = (microblockIndex) / inLineNumber;

	Vector2f result = Vector2f((microblockIndex % inLineNumber) * microsize.x, result.y = (inRawNumber * microsize.y));

	return result;
}

template <class T>
void GridList<T>::fillLocalMatrix(Vector2f targetPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	const int xMicroblocksCount = ceil((bottomRightX - upperLeftX) / microsize.x);
	const int yMicroblocksCount = ceil((bottomRightY - upperLeftY) / microsize.y);

	const int startXInd = upperLeftX / microsize.x;
	const int startYInd = upperLeftY / microsize.y;

	for (int i = startXInd; i < startXInd + xMicroblocksCount; i++)
	{
		for (int j = startYInd; j < startYInd + yMicroblocksCount; j++)
			microblockMatrix[i][j] = 1;
	}

	std::vector<int> blockArea = getBlocksInSight(upperLeftX, upperLeftY, bottomRightX, bottomRightY);

	for (auto&blockInd : blockArea)
	{
		for (auto&item : cells[blockInd])
		{
			auto object = dynamic_cast<TerrainObject*>(item);
			if (!object || object->isBackground)
				continue;

			const int curMicroblockX = int(object->getPosition().x) / microsize.x;
			const int curMicroblockY = int(object->getPosition().y) / microsize.y;

			const int xFillFactor = (object->getRadius() / microsize.x) * 5;
			const int yFillFactor = (object->getRadius() / microsize.y) * 5;

			for (int i = curMicroblockX - xFillFactor; i <= curMicroblockX + xFillFactor; i++)
			{
				for (int j = curMicroblockY - yFillFactor; j <= curMicroblockY + yFillFactor; j++)
				{
					const Vector2f microblockPos = Vector2f(microsize.x * i + microsize.x / 2, microsize.y * j + microsize.y / 2);
					
					if (object->isMultiellipse)
					{
						bool isContinue = false;
						for (int ellipce = 0; ellipce < object->internalEllipses.size(); ellipce++)
						{
							if (Helper::getDist(microblockPos, object->internalEllipses[ellipce].second.first) + Helper::getDist(microblockPos, object->internalEllipses[ellipce].second.second) <= object->internalEllipses[ellipce].first.first)
							{
								microblockMatrix[i][j] = 0;
								isContinue = true;
								break;
							}
						}
						if (isContinue)
							continue;
					}
					else
						if (object->isDotsAdjusted)
						{
							if (!((microblockPos.x >= object->getDot1().x - microsize.x / 2 && microblockPos.x <= object->getDot2().x + microsize.x / 2) &&
								(microblockPos.y >= object->getDot1().y - microsize.y / 2 && microblockPos.y <= object->getDot2().y + microsize.y / 2)))
								continue;

							float t1 = Helper::triangleArea(object->getDot1().x, object->getDot1().y, microblockPos.x - microsize.x / 2, microblockPos.y - microsize.y / 2, object->getDot2().x, object->getDot2().y),
								t2 = Helper::triangleArea(object->getDot1().x, object->getDot1().y, microblockPos.x + microsize.x / 2, microblockPos.y + microsize.y / 2, object->getDot2().x, object->getDot2().y),
								t3 = Helper::triangleArea(object->getDot1().x, object->getDot1().y, microblockPos.x + microsize.x / 2, microblockPos.y - microsize.y / 2, object->getDot2().x, object->getDot2().y),
								t4 = Helper::triangleArea(object->getDot1().x, object->getDot1().y, microblockPos.x - microsize.x / 2, microblockPos.y + microsize.y / 2, object->getDot2().x, object->getDot2().y);

							if (!(Helper::checkSigns(t1, t2) || Helper::checkSigns(t3, t4)))
								microblockMatrix[i][j] = 0;
						}
						else
						{
							if (Helper::getDist(microblockPos, object->getFocus1()) + Helper::getDist(microblockPos, object->getFocus2()) - sqrt(pow(microsize.x, 2) + pow(microsize.y, 2)) > object->getEllipseSize())
								continue;
							if (i >= 0 && j >= 0 && i < microblockMatrix.size() && j < microblockMatrix[0].size())
								microblockMatrix[i][j] = 0;
						}
				}
			}
		}
	}
}

template <class T>
void GridList<T>::makeRoute(Vector2f startPos, Vector2f finishPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	//if (blockArea.size() == 0)
		//return;

	const int xMicroblocksCount = ceil((bottomRightX - upperLeftX) / microsize.x);
	const int yMicroblocksCount = ceil((bottomRightY - upperLeftY) / microsize.y);

	const int startXInd = upperLeftX / microsize.x;
	const int startYInd = upperLeftY / microsize.y;

	int curMicroblockX = startPos.x / microsize.x, curMicroblockY = startPos.y / microsize.y;
	int lastMicroblockX = finishPos.x / microsize.x, lastMicroblockY = finishPos.y / microsize.y;

	for (int i = startXInd; i < startXInd + xMicroblocksCount; i++)
	{
		for (int j = startYInd; j < startYInd + yMicroblocksCount; j++)
		{
			distances[i][j] = inf;
			visited[i][j] = false;
			routes[i][j].clear();			
		}
	}

	/*if (microblockMatrix[curMicroblockX][curMicroblockY] == 0)
	{
		float minD = 10e6;
		int curI = curMicroblockX, curJ = curMicroblockY;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (abs(i) == abs(j))
					continue;
				if (microblockMatrix[curMicroblockX + i][curMicroblockY + j] == 1)
				{
					Vector2f microblockPos = Vector2f(microsize.x * i + microsize.x / 2, microsize.y * j + microsize.y / 2);
					float d = sqrt(pow(startPos.x - microblockPos.x, 2) + pow(startPos.y - microblockPos.y, 2));
					if (d < minD)
					{
						minD = d;
						curI = i;
						curJ = j;
					}
				}
			}
		curMicroblockX += curI;
		curMicroblockY += curJ;
	}*/

	distances[curMicroblockX][curMicroblockY] = 0;
	bfs(startXInd + xMicroblocksCount, startYInd + yMicroblocksCount, curMicroblockX, curMicroblockY, lastMicroblockX, lastMicroblockY);
}

template <class T>
void GridList<T>::bfs(int iBorder, int jBorder, int startX, int startY, int finishX, int finishY)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(startX, startY));
	visited[startX][startY] = true;
	std::vector<std::vector<std::pair<int, int>>> p;
	p.resize(500, std::vector<std::pair<int, int>>(500, {-1, -1}));

	int minDistToFinish = 1000;

	while (!q.empty()) 
	{
		std::pair<int, int> v = q.front();
		q.pop();

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{				
				if (abs(i) == abs(j))
					continue;

				std::pair<int, int> to = std::make_pair(v.first + i, v.second + j);
				if (to.first < 0 || to.first > iBorder || to.second < 0 || to.second > jBorder)
					continue;

				if (!visited[to.first][to.second] && (microblockMatrix[to.first][to.second] != 0 || (to.first == finishX && to.second == finishY)))
				{
					visited[to.first][to.second] = true;
					q.push(to);
					distances[to.first][to.second] = distances[v.first][v.second] + 1;
					p[to.first][to.second] = v;
					if (abs(to.first - finishX) + abs(to.second = finishY) <= minDistToFinish)
					{
						minDistToFinish = abs(to.first - finishX) + abs(to.second = finishY);
						reserveDestination = to;
					}
				}
			}
	}

	bool canCreateRoute = true;

	if (!visited[finishX][finishY])
	{
		finishX = reserveDestination.first; finishY = reserveDestination.second;
		if (!visited[finishX][finishY])
		{
			canCreateRoute = false;
			routes[finishX][finishY].clear();
		}
	}

	if (canCreateRoute)
	{
		std::vector<std::pair<int, int>> path;

		std::pair<int, int> currentMatrixCeil = p[finishX][finishY];
		while (true)
		{
			if (currentMatrixCeil == std::make_pair(-1, -1))
			{
				routes[finishX][finishY].clear();
				return;
			}
			path.push_back(currentMatrixCeil);
			if (currentMatrixCeil == std::make_pair(startX, startY))
				break;
			currentMatrixCeil = p[currentMatrixCeil.first][currentMatrixCeil.second];
		}
		reverse(path.begin(), path.end());


		// cut the corners
		if (path.size() >= 2)
		while (true)
		{
			bool isBreak = true;
			for (int i = 0; i < path.size() - 2; i++)
			{
				if ((path[i].first == path[i + 1].first && path[i].first == path[i + 2].first) ||
					(path[i].second == path[i + 1].second && path[i].second == path[i + 2].second) ||
						(abs(path[i + 1].first - path[i].first) == 1 && abs(path[i + 2].first - path[i + 1].first) == 0 &&
							abs(path[i + 1].second - path[i].second) == 0 && abs(path[i + 2].second - path[i + 1].second) == 1) ||
							(abs(path[i + 1].first - path[i].first) == 0 && abs(path[i + 2].first - path[i + 1].first) == 1 &&
								abs(path[i + 1].second - path[i].second) == 1 && abs(path[i + 2].second - path[i + 1].second) == 0))
				{

					path.erase(path.begin() + (i + 1));
					isBreak = false;
					break;
				}		
			}
			if (isBreak)
				break;
		}
		//----------------

		if (path[0] == std::make_pair(startX, startY))
			path.erase(path.begin() + 0);

		routes[finishX][finishY] = path;
	}
}

template <class T>
void GridList<T>::dfs(int i, int j, int iBorder, int jBorder, int startX, int startY, int finishX, int finishY)
{
	visited[i][j] = true;

	if (i > 0)
		if (microblockMatrix[i - 1][j] != 0 || /*(startX == i - 1 && startY == j) || */(finishX == i - 1 && finishY == j)/* && !visited[i - 1][j]*/)
		{
			if (distances[i - 1][j] > distances[i][j] + 1)
			{
				distances[i - 1][j] = distances[i][j] + 1;
				routes[i - 1][j] = routes[i][j];
				routes[i - 1][j].push_back(std::make_pair(i - 1, j));
				dfs(i - 1, j, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (i < iBorder)
		if (microblockMatrix[i + 1][j] != 0 || /*(startX == i + 1 && startY == j) || */(finishX == i + 1 && finishY == j)/* && !visited[i + 1][j]*/)
		{
			if (distances[i + 1][j] > distances[i][j] + 1)
			{
				distances[i + 1][j] = distances[i][j] + 1;
				routes[i + 1][j] = routes[i][j];
				routes[i + 1][j].push_back(std::make_pair(i + 1, j));
				dfs(i + 1, j, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (j > 0)
		if (microblockMatrix[i][j - 1] != 0 || /*(startX == i && startY == j - 1) || */(finishX == i && finishY == j - 1)/* && !visited[i][j - 1]*/)
		{
			if (distances[i][j - 1] > distances[i][j] + 1)
			{
				distances[i][j - 1] = distances[i][j] + 1;
				routes[i][j - 1] = routes[i][j];
				routes[i][j - 1].push_back(std::make_pair(i, j - 1));
				dfs(i, j - 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (j < jBorder)
		if (microblockMatrix[i][j + 1] != 0 || /*(startX == i && startY == j + 1) || */(finishX == i && finishY == j + 1)/* && !visited[i][j + 1]*/)
		{
			if (distances[i][j + 1] > distances[i][j] + 1)
			{
				distances[i][j + 1] = distances[i][j] + 1;
				routes[i][j + 1] = routes[i][j];
				routes[i][j + 1].push_back(std::make_pair(i, j + 1));
				dfs(i, j + 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (i > 0 && j > 0)
		if (microblockMatrix[i - 1][j - 1] != 0 || /*(startX == i - 1 && startY == j - 1) || */(finishX == i - 1 && finishY == j - 1)/* && !visited[i - 1][j - 1]*/)
		{
			if (distances[i - 1][j - 1] > distances[i][j] + sqrt(2))
			{
				distances[i - 1][j - 1] = distances[i][j] + sqrt(2);
				routes[i - 1][j - 1] = routes[i][j];
				routes[i - 1][j - 1].push_back(std::make_pair(i - 1, j - 1));
				dfs(i - 1, j - 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (i < iBorder && j  > 0)
		if (microblockMatrix[i + 1][j - 1] != 0 || /*(startX == i + 1 && startY == j - 1) || */(finishX == i + 1 && finishY == j - 1)/* && !visited[i + 1][j - 1]*/)
		{
			if (distances[i + 1][j - 1] > distances[i][j] + sqrt(2))
			{
				distances[i + 1][j - 1] = distances[i][j] + sqrt(2);
				routes[i + 1][j - 1] = routes[i][j];
				routes[i + 1][j - 1].push_back(std::make_pair(i + 1, j - 1));
				dfs(i + 1, j - 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (i > 0 && j < jBorder)
		if (microblockMatrix[i - 1][j + 1] != 0 || /*(startX == i - 1 && startY == j + 1) || */(finishX == i - 1 && finishY == j + 1)/* && !visited[i - 1][j + 1]*/)
		{
			if (distances[i - 1][j + 1] > distances[i][j] + sqrt(2))
			{
				distances[i - 1][j + 1] = distances[i][j] + sqrt(2);
				routes[i - 1][j + 1] = routes[i][j];
				routes[i - 1][j + 1].push_back(std::make_pair(i - 1, j + 1));
				dfs(i - 1, j + 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
	if (i < iBorder && j < jBorder)
		if (microblockMatrix[i + 1][j + 1] != 0 || /*(startX == i + 1 && startY == j + 1) || */(finishX == i + 1 && finishY == j + 1)/* && !visited[i + 1][j + 1]*/)
		{
			if (distances[i + 1][j + 1] > distances[i][j] + sqrt(2))
			{
				distances[i + 1][j + 1] = distances[i][j] + sqrt(2);
				routes[i + 1][j + 1] = routes[i][j];
				routes[i + 1][j + 1].push_back(std::make_pair(i + 1, j + 1));
				dfs(i + 1, j + 1, iBorder, jBorder, startX, startY, finishX, finishY);
			}
		}
}

template <class T>
bool GridList<T>::isIntersectWithOthers(Vector2f position1, float radius1, std::vector<WorldObject*> visibleTerrain, bool isDotAdjustded) const
{
	for (auto&anotherItem : visibleTerrain)
	{
		auto const anotherObject = dynamic_cast<WorldObject*>(anotherItem);
		if (!anotherObject)
			continue;

		if (anotherObject->isBackground || anotherObject->tag == Tag::hero1 || anotherObject->intangible)
			continue;

		if (anotherItem->isDotsAdjusted && isDotAdjustded)
			continue;

		Vector2i const position2 = Vector2i(anotherObject->getPosition());
		float const radius2 = anotherObject->getRadius();

		//if (!(abs(position1.x - position2.x) <= (radius1 + radius2) && abs(position1.y - position2.y) <= (radius1 + radius2)))
		if (int(sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2))) < radius1 + radius2)
			return true;
	}

	return false;
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
	for (auto&item : cells[cellIndex])
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
std::vector<T*> GridList<T>::getItems(int blockIndex)
{
	return cells[blockIndex];
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
std::vector<int> GridList<T>::getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int offset)
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
std::vector<int> GridList<T>::getBlocksInSight(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
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

	for (auto&i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (int j = firstColumn; j <= lastColumn; j++)
			result.push_back(j);

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
