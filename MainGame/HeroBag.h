#ifndef HEROBAG_H
#define HEROBAG_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "SpriteStructures.h"
#include "WorldObject.h"

using namespace sf;

enum BagState { bagOpen = 1, bagClosed = 2, bagOpening = 3, bagClosing = 4, ejected = 5 };

struct Cell
{
	Vector2f position;
	std::pair<Tag, int> content;
};

class HeroBag
{
public:
	HeroBag();
	~HeroBag();
	void initialize(Vector2f position, bool isSelectable, std::vector<std::pair<Tag, int>> inventory = testInventory);
	static Cell createCell(Vector2f position, std::pair<Tag, int> content, float radius);
	Vector2f getPosition() const { return position; }
	Vector2f getSizeClosed() const { return sizeClosed; }
	Vector2f getSizeOpen() const { return sizeOpen; }
	Vector2f getLastMousePos() { return lastMousePos; }
	float getRadius();
	int getSelectedCell(Vector2f position);
	bool static canAfford(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem = nullptr);
	void static takeItems(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem = nullptr);
	bool static putItemsIn(std::vector<std::pair<Tag, int>>* loot, std::vector<HeroBag>* bags);
	bool static putItemIn(std::pair<Tag, int>* loot, std::vector<HeroBag>* bags);
	static std::vector<std::pair<Tag, int>> cellsToInventory(std::vector<Cell> cells);
	void mouseMove();
	void setPosition(Vector2f position) { this->position = position; }
	void fixCells();
	void fixPos();

	static std::unordered_map<Tag, int> itemsMaxCount;
	static float itemCommonRadius;
	static std::vector<std::pair<Tag, int>> testInventory, emptyInventory;

	Vector2f textureOpenOffset, textureClosedOffset;
	std::vector<Cell> cells;

	//animation
	void boundSprites(BagSprite* drawInfo);
	void draw(RenderWindow* window, float elapsedTime);
	void drawCircuit(RenderWindow* window);
	void cleanTextureReferences();

	float stateChangeTime = 100000, stateChangingTime = 0, speed = 0.0012f;
	BagState currentState = bagClosed;
	float selectionZoneRadiusClosed, selectionZoneRadiusOpen;
	Vector2f selectionZoneClosedOffset, selectionZoneOpenOffset, shiftVector = { 0, 0 }, movePosition = { 0, 0 };
	bool readyToChangeState = false, readyToEject = false, wasMoved = false, wasBounded = false;
	//---------
private:
	Vector2f position, textureSizeClosed, textureSizeOpen, sizeClosed, sizeOpen, lastMousePos = { 0, 0 };
	Sprite spriteClosedBag, spriteOpenBag, spriteClosedBagSelected, spriteOpenBagSelected;
	Texture* textureClosedBag, *textureOpenBag, *textureClosedBagSelected, *textureOpenBagSelected;
	bool isSelectable;
	std::vector<Vector2f> cellsPos = { {-0.1f, -0.15f}, {0.135f, -0.15f}, {-0.2f, 0.014f}, {0.018f, 0.0f}, {0.241f, 0.004f}, {-0.08f, 0.150f}, {0.131f, 0.150f} };
};

#endif