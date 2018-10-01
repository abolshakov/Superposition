#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <ltbl/lighting/LightSystem.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <windows.h>

#include "GridList.h"
#include "InventoryMaker.h"
#include "Helper.h"
#include "BuildSystemMaker.h"
#include "BoardSprite.h"
#include "EventHandler.h"

#include "DynamicObject.h"

#include "ForestTree.h"
#include "Ground.h"
#include "GroundConnection.h"
#include "Grass.h"
#include "Spawn.h"
#include "BonefireOfInsight.h"
#include "HomeCosiness.h"
#include "MushroomStone.h"
#include "MushroomsOnStone.h"
#include "Chamomile.h"
#include "Yarrow.h"
#include "Brazier.h"
#include "HareTrap.h"

#include "Monster.h"
#include "Deerchant.h"
#include "Wolf.h"
#include "Hare.h"

using namespace sf;

enum staticItemsIdList { tree = 1, grass = 2, spawn = 3, bonefireOfInsight = 4, homeCosiness = 5, mushroomStone = 6, mushroomsOnStone = 7, ground = 11, groundConnection = 12, chamomile = 13, brazier = 14, yarrow = 15, hareTrap = 16 };
enum dynamicItemsIdList { hero1 = 1,  monster = 2, wolf = 3, hare = 4 };

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::unordered_map<std::string, BoardSprite> &spriteMap);

class World
{
private:
	//lightSystem
	const Color commonWorldColor = Color(0, 0, 0, 255),
		commonWorldColorOutfill = Color(240, 200, 200, 255),
		spiritWorldColor = Color(73, 193, 214, 255),
		spiritWorldColorOutfill = Color(12, 78, 89, 255);
	ContextSettings contextSettings;
	sf::RenderStates lightRenderStates;
	sf::Sprite Lsprite;//������ �����.
	Texture pointLightTexture, directionLightTexture;// �������� �����.
	Texture  penumbraTexture;// �������� ��������.
	Shader unshadowShader, lightOverShapeShader;// ������� ��� ������� �����.
	ltbl::LightSystem ls;//���������� ������� ����� � ����.	
	sf::View view;
	std::shared_ptr<ltbl::LightPointEmission> brightner;
	//hero
	const std::string heroTextureName = "Game/worldSprites/hero/stand/down/1.png";
	//world base
	float width, height;
	Vector2i blockSize;
	Vector2f cameraPosition;
	void initSpriteMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = TRUE);
	float World::getScaleFactor();
	Vector2f bossSpawnPosition;
	std::string spriteNameFileDirectory = "Game/objects.txt";
	const float heroToScreenRatio = 0.25f;
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex);
	void World::beyondScreenGenerate();
	int focusedObjectBlock = 0;
	//time logic
	Clock timer;
	int newNameId = 0;
	float timeForNewSave, timeAfterSave;	
	//move logic
	bool isClimbBeyond(Vector2f pos);
	static Vector2f move(DynamicObject& dynamicObject, long long elapsedTime);
	bool isIntersectTerrain(Vector2f newPosition, const TerrainObject& other) const;
	bool isIntersectDynamic(DynamicObject& position, Vector2f newPosition, DynamicObject& other) const;
	static Vector2f newSlippingPositionInCircle(DynamicObject *dynamicItem, Vector2f pos, float radius, long long elapsedTime);
	static Vector2f newSlippingPosition(DynamicObject *dynamicItem, Vector2f pos, long long elapsedTime);
	static Vector2f newSlippingPositionForDynamics(DynamicObject *dynamicItem1, DynamicObject *dynamicItem2, long long elapsedTime);
	//selection logic
	void setTransparent(std::vector<WorldObject*> visibleItems);
	std::string mouseDisplayName;
	WorldObject *selectedObject = focusedObject;
	//shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
	//inventorySystem
	InventoryMaker inventorySystem;
	BuildSystemMaker buildSystem;
	//grids
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	
public:
	World(int width, int height);
	~World();
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
	BuildSystemMaker& getBuildSystem() { return buildSystem; }
	ltbl::LightSystem& getLightSystem() { return ls; }
	//save-load logic
	void ClearWorld();
	void Load();
	void Save();
	void generate(int objCount);
	//base (draw, interact)	
	std::unordered_map<std::string, BoardSprite> spriteMap;
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<WorldObject*> visibleItems);
	Vector2i worldUpperLeft, worldBottomRight;
	//zoom
	float scaleFactor;
	float mainScale;
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;

	Vector2f currentTransparentPos = Vector2f(0, 0);
	//hero
	DynamicObject* focusedObject;	
	StaticObject* groundMatrix[100][100];
	//events
	void onMouseDownInteract(int currentMouseButton);
};

#endif