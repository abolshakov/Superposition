#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>

using namespace sf;

class WorldObject
{
protected:
	std::string name;
	Vector2i textureBoxOffset;
	IntRect textureBox, originalTextureBox;
	Vector2f position;
	int typeOfObject;
	std::string currentWorld;
	int inventoryCapacity;
public:
	int getInvCampacity() { return inventoryCapacity; }
	int getType() { return typeOfObject; }
	bool isVisibleInventory;
	std::vector<std::pair <int, int>> inventory;
	Vector2f biasOffset, bias, lastPosition;	
	Vector2f getBias(Vector2f focusedObjectPos, double elapsedTime);
	Vector2f conditionalSizeUnits;
	std::string getCurrentWorldName() { return currentWorld; }
	bool isTransparent = false;
	bool isTerrain = false;
	bool visited = false;
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();

	virtual void setTextureSize(Vector2i textureSize);
	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();
	Vector2f getPosition() const { return position; }
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }

	const std::string& getName() const { return name; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(int(textureBoxOffset.x), int(textureBoxOffset.y)); }
	virtual std::string getSpriteName(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	void setType(int newType) { typeOfObject = newType; }
	Vector2i getTextureBoxOffset() { return textureBoxOffset; };
	Vector2i getTextureBoxSize() { return Vector2i(textureBox.width, textureBox.height); }
	Vector2f getScaleRatio();
	int varietyOfTypes;
};

#endif