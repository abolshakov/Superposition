#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class WorldObject
{
protected:
	Vector2f position;
	Vector2u size;
	std::string name, type;
public:
	WorldObject(Vector2f position, Vector2u size, std::string name, std::string type);
	virtual ~WorldObject();
	Vector2f getPosition() const { return position;	}
	Vector2u getSize() const { return size; }
	std::string getName() const { return name; }
};

#endif