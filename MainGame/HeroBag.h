#ifndef HEROBAG_H
#define HEROBAG_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Helper.h"

using namespace sf;

enum BagState { bagOpen = 1, bagClosed = 2, bagOpening = 3, bagClosing = 4, ejected = 5 };

struct bagCell
{
	Vector2f position;
	std::pair<lootItemsIdList, int> content;
};

class HeroBag
{
private:
	Vector2f position, textureSizeClosed, textureSizeOpen, sizeClosed, sizeOpen, lastMousePos = {0, 0};
	Sprite spriteClosedBag, spriteOpenBag, spriteClosedBagSelected, spriteOpenBagSelected;
	Texture textureClosedBag, textureOpenBag, textureClosedBagSelected, textureOpenBagSelected;
	bool isSelectable;	
public:
	HeroBag();
	~HeroBag();
	void initialize(Vector2f position, Vector2f sizeClosed, Vector2f sizeOpen, bool isSelectable, std::vector<std::pair<lootItemsIdList, int>> inventory = testInventory, std::string textureClosedBagPath = textureCB, std::string textureClosedBagSelectedPath = textureCBS, std::string textureOpenBagPath = textureOB, std::string textureOpenBagSelectedPath = textureOBS);
	static bagCell createCell(Vector2f position, std::pair<lootItemsIdList, int> content, float radius);
	Vector2f getPosition() const { return position; }
	Vector2f getSizeClosed() const { return sizeClosed; }
	Vector2f getSizeOpen() const { return sizeOpen; }	
	Vector2f getLastMousePos() { return lastMousePos; }
	float getRadius();
	int getSelectedCell(Vector2f position);
	bool static canAfford(std::vector<std::pair<lootItemsIdList, int>> recipe, std::vector<HeroBag>* bags, bagCell* heldItem = nullptr);
	void static takeItems(std::vector<std::pair<lootItemsIdList, int>> recipe, std::vector<HeroBag>* bags, bagCell* heldItem = nullptr);
	bool static putItemsIn(std::vector<std::pair<lootItemsIdList, int>> loot, std::vector<HeroBag>* bags);
	static std::vector<std::pair<lootItemsIdList, int>> cellsToInventory(std::vector<bagCell> cells);
	void setPosition(Vector2f position) { this->position = position; }
	void changeCellsPosition(Vector2f shift);

	static std::unordered_map<lootItemsIdList, int> itemsMaxCount;
	static float itemCommonRadius;
	static std::string textureCB, textureCBS, textureOB, textureOBS;
	static std::vector<std::pair<lootItemsIdList, int>> testInventory, emptyInventory;

	Vector2f textureOpenOffset, textureClosedOffset;
	std::vector<bagCell> cells;

	//animation
	void draw(RenderWindow* window, float elapsedTime, bool canBeMoved = true);
	void drawCircuit(RenderWindow* window);
	float stateChangeTime, stateChangingTime = 0.1f, speed = 0.0012f;
	BagState currentState = bagClosed;
	float selectionZoneRadiusClosed, selectionZoneRadiusOpen;
	Vector2f selectionZoneClosedOffset, selectionZoneOpenOffset, shiftVector = { 0, 0 }, movePosition = {0, 0};
	bool readyToChangeState = false, readyToEject = false, wasMoved = false;
	//---------
};

#endif
