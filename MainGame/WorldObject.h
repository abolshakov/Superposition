#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>

using namespace sf;

enum Tag {emptyTag = 0, mainHeroTag = 1, groundTag = 2, forestTreeTag = 3, chamomileTag = 4, yarrowTag = 5, rockTag = 6, fenceTag = 7, grassTag = 8, stumpTag = 9, buildedObjectTag = 10};
enum State { common = 1, absorbed = 2, constructed = 3 };

class WorldObject
{
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();
	int getZCoords() { return zCoord; }
	int getAnimationLength() { return animationLength; }
	int getInvCapacity() { return inventoryCapacity; }
	int getType() { return typeOfObject; }
	int getVarietyOfTypes() { return varietyOfTypes; }
	float getRotation() { return rotation; }
	bool getDeletePromise() { return delatePromise; }
	int getRadius() { return radius; }
	bool getInventoryVisibility() { return inventoryVisibility; }
	std::string getCurrentWorldName() { return currentWorld; }
	std::string getToSaveName() { return toSaveName; }
	std::string getName() const { return name; }
	virtual std::string getSpriteName(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	Vector2f getPosition() const { return position; }
	Vector2f getPosOffset() { return posOffset; }
	Vector2i getTextureSize() const { return  Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(textureBoxOffset.x, textureBoxOffset.y); }
	Vector2f getScaleRatio();
	Vector2i getConditionalSizeUnits() { return conditionalSizeUnits; }
	virtual Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) = 0;
	virtual int getBuildType(Vector2f ounPos, Vector2f otherPos) = 0;
	IntRect getOriginalTextureBox() { return originalTextureBox; }
	State getState() { return state; }

	void setInventoryVisibility(bool f) { inventoryVisibility = f; }
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }
	void delatePromiseOn() { delatePromise = true; }
	virtual void setTextureSize(Vector2f textureSize);
	void setRotation(int rotation) { this->rotation = rotation; }
	void setState(State state) { this->state = state; }

	bool isTransparent = false, isVisibleName = false, isSelected = false;
	bool isProcessed = false;
	bool isBackground = false, isTerrain = false, isDotsAdjusted = false, isMultiellipse = false;

	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();

	int transparensy = 100;
	int currentBlock = 0;
	std::vector<std::pair <int, int>> inventory;

	Tag tag;
protected:
	int typeOfObject;
	int inventoryCapacity = false;
	int animationLength;
	int zCoord = 0;
	int currentSprite;
	int varietyOfTypes;
	float rotation = 0;
	bool inventoryVisibility = false;
	float timeForNewSprite, animationSpeed;
	std::string name, toSaveName;
	std::string currentWorld;
	IntRect textureBox, originalTextureBox;
	Vector2i randomSizeIncrease;
	Vector2i textureBoxOffset;
	Vector2i conditionalSizeUnits;
	Vector2f position, previousPosition = Vector2f(-1, -1), posOffset = Vector2f(-1, -1);
	int radius;
	bool delatePromise = false;
	State state = common;
};

#endif