#pragma once
#ifndef INVENTORYMAKER_H
#define INVENTORYMAKER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<fstream>
#include "Helper.h"
#include "WorldObject.h"

using namespace sf;

struct cell {
	Sprite sprite;
	Texture texture;
};

class InventoryMaker
{
public:
	InventoryMaker();
	~InventoryMaker();
	void init();
	void drawHeroInventory(float elapsedTime, RenderWindow& window);
	void drawInventory(Vector2f position, float elapsedTime, RenderWindow& window);
	void resetAnimationValues();
	std::unordered_map<int, cell> getSpriteList() { return cellsSpriteList; }
	int getSelectedCellNumber();
	int getHeroInventorySelectedCellNumber();
	int getVisibleItemsNumber() { return visibleItemsNumber; }
	void onMouseDownInteract();
	void putItemToBound();
	void inventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory);
	void temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory);
	bool wasDrawing = false;
	bool getUsedMouse() { return usedMouse; }
	std::pair<int, int> *getHeldItem() { return &heldItem; }
	int heroInventoryRawCellsNumber = 0;
	std::vector<std::reference_wrapper<std::pair <int, int>>> getBoundInv() { return currentInventory; }
	std::vector<int> getItemsMaxCount() { return itemsMaxCount; }
private:
	void initSpriteList();
	int visibleItemsNumber = 0;
	std::string spritesFileDirectory = "Game/inventorySprites/inventorySprites.txt";
	std::unordered_map<int, cell> cellsSpriteList;
	Vector2f cellSize;
	float timeForAnimationEffect = 50000, timeAfterAnimationEffect;
	int animationCounter = 1, currentInventorySize;
	std::vector<std::reference_wrapper<std::pair <int, int>>> currentInventory;
	std::vector<std::reference_wrapper<std::pair <int, int>>> boundInventory;
	//std::vector<std::pair <int, int>>& currentInventory = std::vector<std::pair <int, int>>();
	Vector2f currentPosition, currentHeroInventoryPosition;
	Font font;
	Text numberOfObjects;
	std::vector<int> itemsMaxCount;
	int currentCell = -1 , currentHeroInventoryCell = -1;
	Sprite heroInventoryBackgroundSprite;
	Texture heroInventoryBackgroundTexture;
	Vector2f heroInventoryZoneSize, heroInventoryCellSize;
	std::pair<int, int> heldItem = {-1, -1};
	bool usedMouse = false;
};

#endif