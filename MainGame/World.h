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
#include "Ground.h"
#include "Grass.h"
#include "Spawn.h"
#include "BonefireOfInsight.h"
#include "HomeCosiness.h"
#include "MushroomStone.h"
#include "MushroomsOnStone.h"

#include "Monster.h"
#include "Deerchant.h"
#include "Monster.h"
#include "Wolf.h"

using namespace sf;

enum staticItemsIdList { treeOfGreatness = 1, grass = 2, spawn = 3, bonefireOfInsight = 4, homeCosiness = 5, mushroomStone = 6, mushroomsOnStone = 7, ground = 11};
enum dynamicItemsIdList { hero1 = 1,  monster = 2, wolf = 3};

class World
{
	//lightSystem
	Color commonWorldColor = /*Color(140, 100, 100, 255)*/ Color(0, 0, 0, 255),
		commonWorldColorOutfill = Color(240, 200, 200, 255),
		spiritWorldColor = Color(73, 193, 214, 255),
		spiritWorldColorOutfill = Color(12, 78, 89, 255);
	ContextSettings contextSettings;
	sf::RenderStates lightRenderStates;
	sf::Sprite Lsprite;//Спрайт света.
	Texture pointLightTexture, directionLightTexture;// Текстура света.
	Texture  penumbraTexture;// Текстура полутени.
	Shader unshadowShader, lightOverShapeShader;// Шейдеры для рендера света.
	ltbl::LightSystem ls;//Глобальная система света и тени.	
	sf::View view;
	std::shared_ptr<ltbl::LightPointEmission> brightner;
	//hero
	const std::string heroTextureName = "Maddox/ch1_b_1.png";
	//world base
	float width, height;
	Vector2i blockSize;
	Vector2f cameraPosition;
	void initSpriteMap();
	float World::getScaleFactor();
	Vector2f bossSpawnPosition;
	std::string spriteNameFileDirectory = "World/objects.txt";
	const float heroToScreenRatio = 0.25f;
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex);
	void World::beyondScreenGenerate();
	int focusedObjectBlock = 0;
	//time logic
	Clock timer;
	float timeForNewSave, timeAfterSave;	
	//move logic
	bool isClimbBeyond(Vector2f pos);
	static Vector2f move(DynamicObject& dynamicObject, long long elapsedTime);
	bool isIntersectTerrain(Vector2f newPosition, const TerrainObject& other) const;
	bool isIntersectDynamic(DynamicObject& position, Vector2f newPosition, DynamicObject& other) const;
	static Vector2f newSlippingPositionInCircle(DynamicObject *dynamicItem, Vector2f pos, float radius, long long elapsedTime);
	static Vector2f newSlippingPosition(DynamicObject *dynamicItem, Vector2f pos, long long elapsedTime);
	static Vector2f newSlippingPositionForDynamics(DynamicObject *dynamicItem1, DynamicObject *dynamicItem2, long long elapsedTime);
	//fight logic	
	//selection logic
	void setTransparent(std::vector<WorldObject*> visibleItems);
	std::string mouseDisplayName, mouseSelectedName;
	//shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
	//inventorySystem
	InventoryMaker inventorySystem;
	Builder buildSystem;
	//grids
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	
public:
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
	InventoryMaker& getInventorySystem() { return inventorySystem; }
	Builder& getBuildSystem() { return buildSystem; }
	//save-load logic
	void ClearWorld();
	void Load();
	void Save();
	void generate(int objCount);
	//base (draw, interact)
	World(int width, int height);
	std::unordered_map<std::string, BoardSprite> spriteMap;
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
	void drawVisibleItems(RenderWindow& window, long long elapsedTime);
	std::vector<WorldObject*> visibleItems;
	std::vector<StaticObject*> staticItems;
	Vector2i worldUpperLeft, worldBottomRight;
	//zoom
	float scaleFactor;
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;

	Vector2f currentTransparentPos = Vector2f(0, 0);
	//hero
	DynamicObject* focusedObject;	
	//std::vector< std::vector<StaticObject*> > backgroundMatrix;
	StaticObject* backgroundMatrix[100][100];
	//events
	void onMouseDownInteract();
};

#endif