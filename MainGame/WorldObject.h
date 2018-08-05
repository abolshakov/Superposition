#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>

using namespace sf;

enum Tag {mainHeroTag = 1, groundTag = 2, forestTreeTag = 3, chamomileTag = 4};
enum State { common = 1, absorbed = 2 };

class WorldObject
{
protected:
	int typeOfObject;
	int inventoryCapacity;
	int animationLength;
	int zCoords = 0;
	int currentSprite;
	int varietyOfTypes;
	int rotation = 0;
	bool inventoryVisibility;
	float timeForNewSprite, animationSpeed;
	std::string name, toSaveName;
	std::string currentWorld;
	IntRect textureBox, originalTextureBox;	
	Vector2i randomSizeIncrease;
	Vector2i textureBoxOffset;
	Vector2f conditionalSizeUnits;
	Vector2f position;
	float radius;
	bool delatePromise = false;
	State state = common;
	int brightness = 100; //in percents
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	~WorldObject();
	int getZCoords() { return zCoords; }
	int getAnimationLength() { return animationLength; }
	int getInvCampacity() { return inventoryCapacity; }
	int getType() { return typeOfObject; }
	int getVarietyOfTypes() { return varietyOfTypes; }
	int getRotation() { return rotation; }
	int getDelatePromise() { return delatePromise; }
	int getBrightness() { return brightness; }
	float getRadius() { return radius; }
	bool getInventoryVisibility() { return inventoryVisibility; }
	std::string getCurrentWorldName() { return currentWorld; }
	std::string getToSaveName() { return toSaveName; }
	const std::string& getName() const { return name; }
	virtual std::string getSpriteName(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	Vector2f getPosition() const { return position; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(int(textureBoxOffset.x), int(textureBoxOffset.y)); }
	Vector2f getScaleRatio();
	Vector2f getConditionalSizeUnits() { return conditionalSizeUnits; }
	IntRect getOriginalTextureBox() { return originalTextureBox; }
	State getState() { return state; }

	void setInventoryVisibility(bool f) { inventoryVisibility = f; }
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }
	void delatePromiseOn() { delatePromise = true; }
	virtual void setTextureSize(Vector2i textureSize);
	void setRotation(int rotation) { this->rotation = rotation; }
	void setState(State state) { this->state = state; }

	bool isTransparent = false, isVisibleName = false, isSelected = false;
	bool isBackground = false, isTerrain = false;

	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();	

	int transparensy;
	std::vector<std::pair <int, int>> inventory;

	Tag tag;
};

#endif