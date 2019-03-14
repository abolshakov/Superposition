#ifndef HEROBAG_H
#define HEROBAG_H

#include <SFML/Graphics.hpp>
#include "Helper.h"

using namespace sf;

enum BagState {bagOpen = 1, bagClosed = 2, bagOpening = 3, bagClosing = 4};

struct bagCell
{
	Vector2f position;
	std::pair<int, int> content;
};

class HeroBag
{
private:
	Vector2f position, textureSizeClosed, textureSizeOpen, sizeClosed, sizeOpen;
	Sprite spriteClosedBag, spriteOpenBag, spriteClosedBagSelected, spriteOpenBagSelected;
	Texture textureClosedBag, textureOpenBag, textureClosedBagSelected, textureOpenBagSelected;
	bool isSelectable;
	void changeCellsPosition(Vector2f shift);
	float itemCommonRadius = Helper::GetScreenSize().y / 36;
public:
	HeroBag();
	~HeroBag();
	void initialize(std::string textureClosedBagPath, std::string textureClosedBagSelectedPath, std::string textureOpenBagPath, std::string textureOpenBagSelectedPath, Vector2f position, Vector2f sizeClosed, Vector2f sizeOpen, bool isSelectable);
	static bagCell createCell(Vector2f position, std::pair<int, int> content, float radius);
	Vector2f getPosition() const { return position; }
	Vector2f getSizeClosed() const { return sizeClosed; }
	Vector2f getSizeOpen() const { return sizeOpen; }
	int getSelectedCell(Vector2f position);
	
	Vector2f textureOpenOffset, textureClosedOffset;
	std::vector<bagCell> cells;

	//animation
	void draw(RenderWindow* window, float elapsedTime);
	void drawCircuit(RenderWindow* window);
	float stateChangeTime, stateChangingTime = 0.1f;
	BagState currentState = bagClosed;
	float selectionZoneRadiusClosed, selectionZoneRadiusOpen;
	Vector2f selectionZoneClosedOffset, selectionZoneOpenOffset;
	bool readyToChangeState = false;
	//---------
};

#endif
