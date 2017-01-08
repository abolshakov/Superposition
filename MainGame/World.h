#pragma once
#ifndef WORLD_H
#define WORLD_H


#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "GridList.h"
#include "DynamicObject.h"
#include "Structs.h"
#include "Stone.h"

using namespace sf;
using namespace std;

//struct worldBlock {
//	boardObject activeObjects[300];
//	int activeObjectsCount;
//	int id;
//};

struct boardSprite
{
	string type;
	string name;
	Texture texture;
	Sprite sprite;
};

class World
{
	string nameOfFile;
	int objectsNumber;
	int screenSizeX;
	int screenSizeY;
	int width, height;
	
public:
	GridList<StaticObject> staticObjects;
	GridList<DynamicObject> dynamicObjects;
	int curDraw;
	World(int width, int height);
	void worldGenerate(int objCount);
	unordered_map<string, boardSprite> spriteMap;
	void initSpriteMap();
	void objectsInteract(DynamicObject heroSprite);
	void drawBoard(RenderWindow *window, Vector2f screenSize, DynamicObject heroSprite);
};

#endif