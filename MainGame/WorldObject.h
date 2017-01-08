#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class WorldObject
{
protected:
	Vector2f position;
	Vector2u size;
	string name, type;
public:
	WorldObject(Vector2f position, Vector2u size, string name, string type);
	Vector2f getPosition() const { return position;	}
	Vector2u getSize() const { return size; }
	string getName() const { return name; }
};

#endif