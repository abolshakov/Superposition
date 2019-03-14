#pragma once
#ifndef INVENTORYMAKER_H
#define INVENTORYMAKER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<fstream>
#include "Helper.h"
#include "WorldObject.h"
#include "HeroBag.h"

using namespace sf;

struct cell {
	Sprite sprite;
	Texture texture;
	Vector2f offset;
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
	void onMouseDownInteract();
	void inventoryBounding(std::vector<std::reference_wrapper<HeroBag>> bags);
	void temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory);
	void interact(float elapsedTime);
	bool getUsedMouse() { return usedMouse; }
	bagCell &getHeldItem() { return heldItem; }
	std::unordered_map<int, cell> getSpriteList() { return cellsSpriteList; }

	bool wasDrawing = false;
	std::string debugInfo = "";

	Sprite *selectedCellBackground;
private:
	//hero bags
	std::vector<std::reference_wrapper<HeroBag>> boundBags;
	//another inventories 
	int animationCounter = 1, currentInventorySize;
	float timeForAnimationEffect = 50000, timeAfterAnimationEffect;
	std::vector<std::reference_wrapper<std::pair <int, int>>> currentInventory;
	//held item
	bagCell heldItem;
	float heldItemSpeed = 0;
	//drawing
	std::string spritesFileDirectory = "Game/inventorySprites/inventorySprites.txt";
	std::unordered_map<int, cell> cellsSpriteList;
	void initSpriteList();
	void drawNumberOfItems(Sprite sprite, int itemsCount, RenderWindow &window);
	Font font;
	Text numberOfItems;

	std::vector<int> itemsMaxCount;
	bool usedMouse = false;
	float itemCommonRadius = 0;

	//std::vector<std::pair<Vector2f, std::string>> temporaryPositions;
};

#endif