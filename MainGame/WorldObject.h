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
public:
	WorldObject(std::string objectName, Vector2f ellipseCenterPosition);
	virtual ~WorldObject();

	virtual void setTextureSize(Vector2f textureSize);
	virtual Vector2f calculateTextureOffset();	
	virtual void initEllipse();
	//Vector2f getPosition() const { return position; }
	const std::string& getName() const { return name; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }	
	Vector2i getTexturePosition() const { return Vector2i(textureBox.left, textureBox.top); }
	virtual const std::string getSpriteName(long long elapsedTime) = 0;
};

#endif