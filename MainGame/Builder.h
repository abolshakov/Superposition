#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Helper.h"
#include "InventoryMaker.h"

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
	Builder(std::unordered_map<int, cell> itemsSpriteList);
	void draw(RenderWindow &window, World& world, float elapsedTime);
	void onMouseDownInteract(World& world);
	void interact();
	void setCharacterBuildActivity(Deerchant& hero);
	int getCurrentObject() { return currentObject; }
private:	
	std::unordered_map<int, cell> craftIngredientsSpriteList;
	Vector2f currentSpritePos, currentSpriteOffset, currentSpriteSize, currentSpriteScale;
	void initializeButtons();
	void inicializeObjectsInfo();
	void animator(float elapsedTime);
	void buildObject(Vector2f position, World& world);
	float originalAnimationSpeed = 0.001, animationSpeed = 0.001;
	bool isBuilding = false, isPressed = false, buildingAvaliable = false, isRecipeFrame;
	Sprite buildStartButton, buildStopButton, recipeFrame;
	std::vector<objectInfo> builtObjects;
	Texture buildStartButtonTexture, buildStopButtonTexture, recipeFrameTexture;
	int currentObject = 0;
	std::string builderObjectsInfoFileDirectory = "World/builder/builderObjectsInfo.txt";
};

