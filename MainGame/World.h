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
	
public:
	World(int width, int height);
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	void worldGenerate(int objCount);
	std::unordered_map<std::string, boardSprite> spriteMap;
	void initSpriteMap();
	void objectsInteract(DynamicObject heroSprite);
	void drawBoard(RenderWindow* window, Vector2f screenSize, DynamicObject* heroSprite);
};

#endif