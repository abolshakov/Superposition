#pragma once
#ifndef INVENTORYMAKER_H
#define INVENTORYMAKER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<fstream>
#include "Helper.h"
#include "TextWriter.h"
#include "UIEffectsSystemMaker.h"
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
	void inventoryBounding(std::vector<HeroBag>* bags);
	void temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <Tag, int>>> inventory);
	void interact(float elapsedTime);
	bool getUsedMouse() { return usedMouse; }	
	Cell &getHeldItem() { return heldItem; }
	std::unordered_map<Tag, cell> getSpriteList() { return cellsSpriteList; }

	bool wasDrawing = false;
	std::string debugInfo = "", cursorText = "";

	Sprite *selectedCellBackground, dropZone;
	CircleShape bagPosDot;
	Texture dropZoneTexture;
private:
	//hero bags
	std::vector<HeroBag>* boundBags;
	int currentMovingBag = -1;
	float minDistToClosed = 10e4, minDistToOpen = 10e4;

	//another inventories 
	int animationCounter = 1, currentInventorySize;
	float timeForAnimationEffect = 50000, timeAfterAnimationEffect;
	std::vector<std::reference_wrapper<std::pair <Tag, int>>> currentInventory;
	//held item
	Cell heldItem;
	float heldItemSpeed = 0, dropZoneRadius;
	//drawing
	std::string spritesFileDirectory = "Game/inventorySprites/inventorySprites.txt";
	std::unordered_map<Tag, cell> cellsSpriteList;
	void initSpriteList();
	void drawNumberOfItems(Sprite sprite, int itemsCount, RenderWindow &window);
	void crashIntoOtherBags(int cnt);
	std::vector<bool> visitedInDisplacement;
	Font font;
	Text numberOfItems;
	bool usedMouse = false, cursorBlurUsing = false;
	Vector2f cursorTextPos = {0, 0};
	TextWriter textWriter;
	UIEffectsSystemMaker uiEffectsSystem;
	//std::vector<std::pair<Vector2f, std::string>> temporaryPositions;
};

#endif