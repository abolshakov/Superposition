#ifndef BUILDSYSTEMMAKER_H
#define BUILDSYSTEMMAKER_H

#include <SFML/Graphics.hpp>
#include "Helper.h"
#include "WorldObject.h"
#include "InventoryMaker.h"
#include "GridList.h"
#include "SpriteStructures.h"

using namespace sf;

struct objectInfo {
	std::string type, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	std::vector<std::pair<Tag, int>> recipe;
};

class BuildSystemMaker
{
protected:
	void drawRecipeFrame(RenderWindow &window);
public:	
	BuildSystemMaker();
	~BuildSystemMaker();
	void inventoryBounding(std::vector<HeroBag>* boundBags);
	typedef std::vector<std::pair <int, int>> datatype;
	datatype blankref;
	std::vector<HeroBag>* boundBags;

	void Init(const std::unordered_map<Tag, CellSprite>& itemsSpriteList);
	void draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f cameraPosition, std::vector<WorldObject*> visibleItems, bool showPositioning = true);
	void onMouseUp(Vector2f focusedObjectPosition, float scaleFactor);
	void buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor);
	void interact();
	int getCurrentObject() { return currentObject; }
	int getBuildType() { return buildType; }
	void setCurrentObject(int n) { currentObject = n; }
	void wasPlaced();
	Vector2f buildingPosition = {-1, -1};
	bool canAfford();
	bool succesInit = false;
	bool getUsedMouse() { return usedMouse; }
	bool canBePlaced = false;
	Tag selectedObject = Tag::emptyCell;
	void setHeldItem(std::pair<Tag, int>& heldItem) { if (nullptr != &heldItem) this->heldItem = &heldItem; }
	void clearHareBags(int block, GridList<StaticObject>& staticGrid, std::vector<WorldObject*>* visibleItems);
	std::map<Tag, bool> droppedLootIdList = { {Tag::heroBag, 1} };
private:	
	std::unordered_map<Tag, CellSprite> craftIngredientsSpriteList;
	std::pair<Tag, int> *heldItem = nullptr;
	Vector2f mouseWorldPos = { 0, 0 };
	Vector2f spriteBuildPos = Vector2f (-1, -1);
	int buildType = 1;
	void initializeButtons();
	void inicializeObjectsInfo();
	void animator(float elapsedTime);
	float originalAnimationSpeed = 0.001f, animationSpeed = 0.001f;
	bool isPressed = false, isRecipeFrame, usedMouse = false;
	Sprite buildStartButton, buildStopButton, recipeFrame;
	std::vector<objectInfo> builtObjects;
	Texture buildStartButtonTexture, buildStopButtonTexture, recipeFrameTexture;
	int currentObject = -1;
	std::string BuildSystemMakerObjectsInfoFileDirectory = "Game/buildSystem/buildSystemObjectsInfo.txt";
	Text numberOfObjects;
	Font font;
};

#endif