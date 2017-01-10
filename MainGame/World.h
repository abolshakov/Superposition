#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include "GridList.h"
#include "DynamicObject.h"
#include "Stone.h"

using namespace sf;

struct boardSprite
{
	std::string type;
	std::string name;
	Texture texture;
	Sprite sprite;
};

class World
{
	std::string nameOfFile;
	int screenSizeX;
	int screenSizeY;
	int width, height;
	static Vector2f move(const DynamicObject& dynamicObject, long long elapsedTime);
public:
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	DynamicObject* focusedObject;
	std::unordered_map<std::string, boardSprite> spriteMap;

	World(int width, int height);
	
	void initSpriteMap();
	void generate(int objCount);
	void interact(long long elapsedTime) const;
	void draw(RenderWindow& window, long long elapsedTime);	
};

#endif