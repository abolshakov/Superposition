#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class WorldObject
{
protected:
	std::string name;
	Vector2f textureBoxOffset;
	IntRect textureBox;
	Vector2f smoothBorderX, smoothBorderY, position;
	std::string typeOfImage;
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();

	virtual void setTextureSize(Vector2f textureSize);
	virtual Vector2f calculateTextureOffset();	
	virtual void initPedestal();
	Vector2f getPosition() const { return position; }
	void setPosition(Vector2f newPosition);

	const std::string& getName() const { return name; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }	
	Vector2i getTextureOffset() const { return Vector2i(textureBoxOffset.x, textureBoxOffset.y); }
	virtual const std::string getSpriteName(long long elapsedTime) = 0;
	void setTypeOfImage(std::string newType) { typeOfImage = newType; }
};

#endif