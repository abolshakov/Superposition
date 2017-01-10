#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class WorldObject
{
protected:
	Vector2u size;
	std::string name;
public:
	Vector2f position;

	WorldObject(Vector2f position, Vector2u size, std::string name);
	virtual ~WorldObject();

	virtual std::string getSpriteName(long long elapsedTime) = 0;
	Vector2u getSize() const { return size; }
	std::string getName() const { return name; }
};

#endif