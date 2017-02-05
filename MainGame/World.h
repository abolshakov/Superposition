#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include "GridList.h"
#include "DynamicObject.h"
#include "RoseTree.h"

using namespace sf;

struct boardSprite
{
	Texture texture;
	Sprite sprite;
};

class World
{
	const std::string heroName = "hero";
	const std::string heroTextureName = "heroF_0.png";
	const float heroToScreenRatio = 0.25f;

	std::string nameOfFile;	
	int width, height;

	Vector2i initSpriteMap();
	float World::getScaleFactor();
	static Vector2f move(const DynamicObject& dynamicObject, long long elapsedTime);
	bool isIntersectTerrain(Vector2f position, const TerrainObject& other) const;
	bool isIntersectDynamic(Vector2f position, const DynamicObject& other) const;
	static Vector2f newSlippingPosition(DynamicObject *dynamicItem, TerrainObject *terrain, Vector2f newPosition, long long elapsedTime);
public:
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	DynamicObject* focusedObject;
	std::unordered_map<std::string, boardSprite> spriteMap;
	float scaleFactor;

	World(int width, int height);

	void generate(int objCount);
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
};

#endif