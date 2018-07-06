#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>

using namespace sf;

class WorldObject
{
protected:
	int typeOfObject;
	int inventoryCapacity;
	int animationLength;
	int zCoords = 0;
	int currentSprite;
	int varietyOfTypes;
	bool inventoryVisibility;
	float timeForNewSprite, animationSpeed;
	std::string name, toSaveName;
	std::string currentWorld;
	IntRect textureBox, originalTextureBox;	
	Vector2i randomSizeIncrease;
	Vector2i textureBoxOffset;
	Vector2f conditionalSizeUnits;
	Vector2f position;
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();

	int getZCoords() { return zCoords; }
	int getAnimationLength() { return animationLength; }
	int getInvCampacity() { return inventoryCapacity; }
	int getType() { return typeOfObject; }
	int getVarietyOfTypes() { return varietyOfTypes; }
	bool getInventoryVisibility() { return inventoryVisibility; }
	std::string getCurrentWorldName() { return currentWorld; }
	std::string getToSaveName() { return toSaveName; }
	const std::string& getName() const { return name; }
	virtual std::string getSpriteName(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	Vector2f getPosition() const { return position; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(int(textureBoxOffset.x), int(textureBoxOffset.y)); }
	Vector2i getTextureBoxOffset() { return textureBoxOffset; };
	Vector2i getTextureBoxSize() { return Vector2i(textureBox.width, textureBox.height); }
	Vector2f getScaleRatio();
	Vector2f getConditionalSizeUnits() { return conditionalSizeUnits; }
	IntRect getOriginalTextureBox() { return originalTextureBox; }

	void setInventoryVisibility(bool f) { inventoryVisibility = f; }

	std::vector<std::pair <int, int>> inventory;	
	
	bool isTransparent = false, isVisibleName = false, isSelected = false;
	bool isBackground = false, isTerrain = false;

	virtual void setTextureSize(Vector2i textureSize);
	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();
	
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }

	int transparensy;

	std::string tag;
};

#endif