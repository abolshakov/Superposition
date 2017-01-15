#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class WorldObject
{
protected:
	std::string name;
	Vector2i size;
	Vector2f textureBoxOffset;
	FloatRect box;
	IntRect textureBox;
public:
	WorldObject(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize);
	virtual ~WorldObject();

	const std::string& getName() const { return name; }
	Vector2f getPosition() const { return Vector2f(box.left, box.top); }
	void setPosition(Vector2f newPosition)
	{
		box.left = newPosition.x;
		box.top = newPosition.y; // Introduce Texture Offset
		textureBox.left = newPosition.x - textureBoxOffset.x;
		textureBox.top = newPosition.y - textureBoxOffset.y;
	}
	Vector2i getSize() const { return size; }
	const FloatRect& getBoundingBox() const { return box; }
	Vector2i getTextureSize() const { return Vector2i(textureBox.width, textureBox.height); }	
	Vector2i getTexturePosition() const { return Vector2i(textureBox.left, textureBox.top); }
	virtual const std::string getSpriteName(long long elapsedTime) = 0;
};

#endif