#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include "GridList.h"
#include "DynamicObject.h"
#include "Stone.h"
#include "RoseTree.h"

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
	bool isIntersectTerrain(Vector2f position, const DynamicObject& dynamic, const TerrainObject& other);
	bool isIntersectDynamic(Vector2f position, const DynamicObject& dynamic, const DynamicObject& other);
	Vector2f newSlippingPosition(DynamicObject *dynamicItem, TerrainObject *terrain, Vector2f newPosition, float elapsedTime);
public:
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	DynamicObject* focusedObject;
	std::unordered_map<std::string, boardSprite> spriteMap;

	World(int width, int height);
	
	void initSpriteMap();
	void generate(int objCount);
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);	
};

#endif