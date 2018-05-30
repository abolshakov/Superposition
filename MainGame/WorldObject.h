#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>

using namespace sf;

class WorldObject
{
protected:
	std::string name, toSaveName;
	Vector2i textureBoxOffset;
	IntRect textureBox, originalTextureBox;
	Vector2f position;
	std::string currentWorld;
	int typeOfObject;
	int inventoryCapacity;
	int animationLength;
	bool isVisibleInventory;
	int zCoords = 0;
	int currentSprite;
	float timeForNewSprite, animationSpeed;
	Vector2i randomSizeIncrease;
public:
	//getters
	bool getIsVisibleInventory() { return isVisibleInventory; }
	int getZCoords() { return zCoords; }
	int getAnimationLength() { return animationLength; }
	int getInvCampacity() { return inventoryCapacity; }
	int getType() { return typeOfObject; }
	std::string getCurrentWorldName() { return currentWorld; }
	std::string getToSaveName() { return toSaveName; }
	const std::string& getName() const { return name; }
	virtual std::string getSpriteName(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	Vector2f getBias(Vector2f focusedObjectPos, double elapsedTime);
	Vector2f getPosition() const { return position; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(int(textureBoxOffset.x), int(textureBoxOffset.y)); }
	Vector2i getTextureBoxOffset() { return textureBoxOffset; };
	Vector2i getTextureBoxSize() { return Vector2i(textureBox.width, textureBox.height); }
	Vector2f getScaleRatio();
	IntRect getOriginalTextureBox() { return originalTextureBox; }

	void setIsVisibleInventory(bool f) { isVisibleInventory = f; }

	std::vector<std::pair <int, int>> inventory;
	Vector2f biasOffset, bias, lastPosition;	
	
	bool isIntersectsWithOther = false;
	Vector2f conditionalSizeUnits;
	
	bool isTransparent = false, isVisibleName = false, isSelected = false;
	bool isBackground = false, isTerrain = false;
	bool visited = false;
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();

	virtual void setTextureSize(Vector2i textureSize);
	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();
	
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }

	int transparensy;
	int varietyOfTypes;
};

#endif