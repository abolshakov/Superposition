#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <SFML/Graphics.hpp>
#include "Helper.h"
#include "WorldObject.h"
#include "InventoryMaker.h"
#include "GridList.h"
#include "BoardSprite.h"

using namespace sf;

struct objectInfo {
	std::string type, image, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	std::vector<std::pair<int, int>> recipe;
};

class Builder
{
protected:

public:	
	Builder();

	void inventoryBounding(std::vector<std::pair <int, int>>& inventory);
	typedef std::vector<std::pair <int, int>> datatype;
	datatype blankref;
	std::reference_wrapper<datatype> currentInventory = blankref;

	void Init(std::unordered_map<int, cell> itemsSpriteList);
	void draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f focusedObjectPosition, std::vector<WorldObject*> visibleItems);
	void onMouseDownInteract(Vector2f focusedObjectPosition, float scaleFactor);
	void interact();
	int getCurrentObject() { return currentObject; }
	void setCurrentObject(int n) { currentObject = n; }
	void wasPlaced();
	int getBuiltObjectType();
	bool canAfford();
	Vector2f buildingPosition;
	bool succesInit = false;
	bool getIsBuilding() { return isBuilding; }
	
	bool canBePlaced = false;
	int selectedObject = -1;
private:	
	std::unordered_map<int, cell> craftIngredientsSpriteList;
	Vector2f currentSpritePos, currentSpriteOffset, currentSpriteSize, currentSpriteScale;
	void initializeButtons();
	void inicializeObjectsInfo();
	void animator(float elapsedTime);
	//void buildObject(Vector2f position, World& world);
	float originalAnimationSpeed = 0.001, animationSpeed = 0.001;
	bool isBuilding = false, isPressed = false, isRecipeFrame;
	Sprite buildStartButton, buildStopButton, recipeFrame;
	std::vector<objectInfo> builtObjects;
	Texture buildStartButtonTexture, buildStopButtonTexture, recipeFrameTexture;
	int currentObject = -1;
	std::string builderObjectsInfoFileDirectory = "World/builder/builderObjectsInfo.txt";
	Text numberOfObjects;
	Font font;
};

#endif