#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <ltbl/lighting/LightSystem.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

#include "GridList.h"
#include "InventoryMaker.h"
#include "Helper.h"
#include "Builder.h"
#include "BoardSprite.h"

#include "DynamicObject.h"
#include "TerrainObject.h"

#include "TreeOfGreatness.h"
#include "Grass.h"
#include "Spawn.h"
#include "BonefireOfInsight.h"
#include "HomeCosiness.h"
#include "MushroomStone.h"

#include "Enemy.h"
#include "Deerchant.h"

using namespace sf;

enum VictimSide { upSide = 1, rightSide = 2, downSide = 3, leftSide = 4 };
enum staticItemsIdList { treeOfGreatness = 1, grass = 2, spawn = 3, bonefireOfInsight = 4, homeCosiness = 5, mushroomStone = 6};
enum dynamicItemsIdList { hero1 = 1, enemy = 2};

class World
{
	//lightSystem
	Color commonWorldColor = Color(140, 90, 90, 255), commonWorldColorOutfill = Color(229.5, 178, 178, 255), spiritWorldColor = Color(73, 193, 214, 255), spiritWorldColorOutfill = Color(12, 78, 89, 255);
	ContextSettings contextSettings;
	sf::RenderStates lightRenderStates;
	sf::Sprite Lsprite;//Спрайт света.
	Texture pointLightTexture, ConeLightTexture;// Текстура света.
	Texture  penumbraTexture;// Текстура полутени.
	Shader unshadowShader, lightOverShapeShader;// Шейдеры для рендера света.
	ltbl::LightSystem ls;//Глобальная система света и тени.	
	sf::View view;
	//hero
	const std::string heroTextureName = "Maddox/ch1_b_1.png";
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
	bool isIntersectTerrain(Vector2f newPosition, const TerrainObject& other) const;
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
	//inventorySystem
	InventoryMaker inventorySystem;
	Builder buildSystem;
	//test
	Vector2f positioning, lastPosition;
	std::string testString;
	//lightSystem
	void initLightSystem(RenderWindow &window);
	void renderLightSystem(View view, RenderWindow &window);
	//adding to the grid
	void initializeStaticItem(staticItemsIdList itemClass, Vector2f itemPosition, int itemType, std::string itemName);
	void initializeDynamicItem(dynamicItemsIdList itemClass, Vector2f itemPosition, std::string itemName);
	//getters
	Vector2f getBossSpawnPosition() { return bossSpawnPosition; }
	Vector2i getWorldSize() { return Vector2i(width, height); }
	GridList<StaticObject> getStaticGrid() { return staticGrid; }
	GridList<DynamicObject> getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() { return cameraPosition; }
	InventoryMaker getInventorySystem() { return inventorySystem; }
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
	std::unordered_map<std::string, BoardSprite> spriteMap;
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
	void heroInteractWithMobs(DynamicObject& victim, float elapsedTime);
	void hitInteract(DynamicObject& currentItem, float elapsedTime);
	Vector2f currentTransparentPos = Vector2f(0, 0);
	VictimSide victimSide;
	//hero
	//const std::string heroName = "hero";
	DynamicObject* focusedObject;	
};

#endif