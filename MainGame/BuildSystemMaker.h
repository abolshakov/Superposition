#ifndef BUILDSYSTEMMAKER_H
#define BUILDSYSTEMMAKER_H

#include <SFML/Graphics.hpp>
#include "Helper.h"
#include "WorldObject.h"
#include "InventoryMaker.h"
#include "GridList.h"
#include "BoardSprite.h"

using namespace sf;

struct objectInfo {
	std::string type, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	std::vector<std::pair<int, int>> recipe;
};

class BuildSystemMaker
{
protected:
	void drawRecipeFrame(RenderWindow &window);
public:	
	BuildSystemMaker();
	~BuildSystemMaker();
	void inventoryBounding(std::vector<std::reference_wrapper<HeroBag>> bags);
	typedef std::vector<std::pair <int, int>> datatype;
	datatype blankref;
	std::vector<std::reference_wrapper<HeroBag>> boundBags;

	void Init(const std::unordered_map<int, cell>& itemsSpriteList);
	void draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f cameraPosition, std::vector<WorldObject*> visibleItems, bool showPositioning = true);
	void onMouseDownInteract(Vector2f focusedObjectPosition, float scaleFactor);
	void buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor);
	void interact();
	int getCurrentObject() { return currentObject; }
	int getBuildType() { return buildType; }
	void setCurrentObject(int n) { currentObject = n; }
	void wasPlaced();
	int getBuiltObjectType();
	Vector2f buildingPosition = {-1, -1};
	bool canAfford();
	bool succesInit = false;
	bool getIsBuilding() { return isBuilding; }
	bool getUsedMouse() { return usedMouse; }
	bool canBePlaced = false;
	int selectedObject = -1;
	void setHeldItem(std::pair<int, int>& heldItem) { if (nullptr != &heldItem) this->heldItem = &heldItem; }
private:	
	std::unordered_map<int, cell> craftIngredientsSpriteList;
	std::pair<int, int> *heldItem = nullptr;	
	Vector2f spriteBuildPos = Vector2f (-1, -1);
	int buildType = 1;
	void initializeButtons();
	void inicializeObjectsInfo();
	void animator(float elapsedTime);
	float originalAnimationSpeed = 0.001f, animationSpeed = 0.001f;
	bool isBuilding = false, isPressed = false, isRecipeFrame, usedMouse = false;
	Sprite buildStartButton, buildStopButton, recipeFrame;
	std::vector<objectInfo> builtObjects;
	Texture buildStartButtonTexture, buildStopButtonTexture, recipeFrameTexture;
	int currentObject = -1;
	std::string BuildSystemMakerObjectsInfoFileDirectory = "Game/buildSystem/buildSystemObjectsInfo.txt";
	Text numberOfObjects;
	Font font;
};

#endif