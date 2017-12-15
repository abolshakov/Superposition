#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include "GridList.h"
#include "DynamicObject.h"
#include "RoseTree.h"
#include "Enemy.h"
#include "Deerchant.h"

using namespace sf;

struct boardSprite
{
	Texture texture;
	Sprite sprite;
};

enum VictimSide { upSide = 1, rightSide = 2, downSide = 3, leftSide = 4 };

class World
{
	//hero
	const std::string heroTextureName = "hero/heroF_0.png";
	//world base
	float width, height;
	Vector2i blockSize;
	Vector2f cameraPosition;
	Vector2i initSpriteMap();
	float World::getScaleFactor();
	Vector2f bossSpawnPosition;
	std::string spriteNameFileDirectory = "World/objects.txt";
	const float heroToScreenRatio = 0.25f;
	//time logic
	Clock timer;
	float timeForNewSave, timeAfterSave;	
	//move logic
	bool isClimbBeyond(Vector2f pos);
	static Vector2f move(const DynamicObject& dynamicObject, long long elapsedTime);
	bool isIntersectTerrain(DynamicObject& dynamic1, Vector2f newPosition, const TerrainObject& other) const;
	bool isIntersectDynamic(DynamicObject& position, Vector2f newPosition, const DynamicObject& other) const;
	static Vector2f newSlippingPositionInCircle(DynamicObject *dynamicItem, Vector2f pos, float radius, long long elapsedTime);
	static Vector2f newSlippingPosition(DynamicObject *dynamicItem, Vector2f pos, long long elapsedTime);
	static Vector2f newSlippingPositionForDynamics(DynamicObject *dynamicItem1, DynamicObject *dynamicItem2, long long elapsedTime);
	//fight logic
	VictimSide getVictimSide(DynamicObject& hero, DynamicObject& victim);		
	//selection logic
	void setTransparent(std::vector<WorldObject*> visibleItems);
	std::string mouseDisplayName;
	//shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
public:
	//test
	Vector2f positioning, lastPosition;
	//adding to the grid
	void initializeRoseTree(Vector2f position, int typeOfImage, std::string itemName);
	void initializeSpawn(Vector2f position, int typeOfImage);
	void initializeGrass(Vector2f position, int typeOfImage, std::string itemName);
	void initializeEnemy(Vector2f position);
	void initializeHero(Vector2f position);
	//getters
	Vector2f getBossSpawnPosition() { return bossSpawnPosition; }
	Vector2i getWorldSize() { return Vector2i(width, height); }
	GridList<StaticObject> getStaticGrid() { return staticGrid; }
	GridList<DynamicObject> getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() { return cameraPosition; }
	//save-load logic
	void ClearWorld();
	void Load();
	void Save();
	void generate(int objCount);
	//grids
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	//base (draw, interact)
	World(int width, int height);
	std::unordered_map<std::string, boardSprite> spriteMap;
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
	std::vector<WorldObject*> visibleItems;
	std::vector<StaticObject*> staticItems;
	Vector2i worldUpperLeft, worldBottomRight;
	//zoom
	float scaleFactor;
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;
	//fight logic
	void heroHit(DynamicObject& heroObject, DynamicObject& victim, float elapsedTime);
	void hitInteract(DynamicObject& currentItem, float elapsedTime);
	Vector2f currentTransparentPos = Vector2f(0, 0);
	VictimSide victimSide;
	//hero
	const std::string heroName = "hero";
	DynamicObject* focusedObject;	
};

#endif