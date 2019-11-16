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
#include "EffectsSystemMaker.h"
#include "SpriteStructures.h"
#include "EventHandler.h"

#include "DynamicObject.h"
#include "EmptyObject.h"

#include "ForestTree.h"
#include "Brazier.h"

using namespace sf;

enum Biomes {mud = 1, highGrass = 2, grassWithFlowers = 3, oldMud = 4 };

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::unordered_map<std::string, BoardSprite> &spriteMap);
typedef bool(*func)(Vector2f &pos);

struct BiomesMapCell
{
	StaticObject* groundCell;
	Biomes biomeCell;
};

class World
{
public:
	World(int width, int height);
	~World();

	//lightSystem
	void initLightSystem(RenderWindow &window);
	void renderLightSystem(View view, RenderWindow &window);

	//adding to the grid
	void birthObjects();
	void initializeStaticItem(Tag itemClass, Vector2f itemPosition, int itemType, std::string itemName, bool reliable, int count = 1, std::vector<std::pair<Tag, int>> inventory = {});
	void initializeDynamicItem(Tag itemClass, Vector2f itemPosition, std::string itemName, WorldObject* owner = nullptr);

	//getters
	Vector2f getWorldSize() { return Vector2f (width, height); }
	GridList<StaticObject> getStaticGrid() { return staticGrid; }
	GridList<DynamicObject> getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() { return cameraPosition; }
	InventoryMaker& getInventorySystem() { return inventorySystem; }
	BuildSystemMaker& getBuildSystem() { return buildSystem; }
	ltbl::LightSystem& getLightSystem() { return ls; }
	std::string getMouseDisplayName() const { return mouseDisplayName; }
	WorldObject* getSelectedObject() const { return selectedObject; }

	//save-load logic
	void clearWorld();
	void Load();
	void Save();
	void generate(int objCount);

	//base (draw, interact)	
	std::unordered_map<std::string, BoardSprite> spriteMap;
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<spriteChainElement> sprites);
	Vector2f worldUpperLeft, worldBottomRight;
	void runBuildSystemDrawing(RenderWindow& window, float elapsedTime);
	void runInventorySystemDrawing(RenderWindow& window, float elapsedTime);

	//zoom
	float scaleFactor;
	float mainScale;
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;
	std::map<Tag, bool> unscaledObjects = {{Tag::hero1, true}, {Tag::nightmare1, true}, {Tag::nightmare2, true}, {Tag::nightmare3, true} };

	//hero
	DynamicObject* focusedObject;
	Brazier* brazier;
	BiomesMapCell biomeMatrix[100][100];	
	//events
	void onMouseUp(int currentMouseButton);
	bool getHeroBookVisability() { return isHeroBookVisible; }
	void changeBookVisability() { isHeroBookVisible = !isHeroBookVisible; }

	Vector2i currentTransparentPos = Vector2i(0, 0);
private:
	//lightSystem
	const Color commonWorldColor = Color(0, 0, 0, 255),
		//commonWorldColorOutfill = Color(240, 200, 200, 255),
		commonWorldColorOutfill = Color(255, 255, 255, 255),
		spiritWorldColor = Color(73, 193, 214, 255),
		spiritWorldColorOutfill = Color(12, 78, 89, 255);
	ContextSettings contextSettings;
	sf::RenderStates lightRenderStates;
	sf::Sprite Lsprite;//������ �����.
	Texture pointLightTexture, directionLightTexture;// �������� �����.
	Texture penumbraTexture;// �������� ��������.
	Shader unshadowShader, lightOverShapeShader;// ������� ��� ������� �����.
	ltbl::LightSystem ls;//���������� ������� ����� � ����.	
	sf::View view;
	std::shared_ptr<ltbl::LightPointEmission> brightner;

	//hero
	const std::string heroTextureName = "Game/worldSprites/hero/move/body/down/1.png";
	bool isHeroBookVisible = false;

	//world base
	float width, height;
	Vector2i blockSize, microblockSize;
	Vector2f cameraPosition, maxCameraDistance = Vector2f(100, 100), camOffset = { 0, -0.04f};
	void initSpriteMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	float World::getScaleFactor();
	std::string spriteNameFileDirectory = "Game/objects.txt";
	const float heroToScreenRatio = 0.25f;
	Vector2i focusedObjectBlock = Vector2i(0, 0), BiomesChangeCenter = Vector2i(0, 0);
	bool fixedClimbingBeyond(Vector2f &pos);
	void makeCameraShake(float power = 0.0002f);
	void cameraShakeInteract(float elapsedTime);
	Vector2f cameraShakeVector = { 0, 0 };
	float cameraShakeDuration = 0, cameraShakePower = 0.0002f;

	//active generation
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex);
	void BiomesGenerate(int offset);
	void perimeterGeneration(int offset);
	void beyondScreenGeneration();

	//time logic
	Clock timer;
	int newNameId = 10;
	int biomeGenerateDistance = 4;
	float timeForNewSave, timeAfterSave;
	const float timeForNewRotutes = 5000000;

	//selection logic
	void setTransparent(std::vector<WorldObject*> visibleItems, float elapsedTime);
	std::string mouseDisplayName;
	WorldObject *selectedObject = focusedObject;

	//shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
	EffectsSystemMaker effectSystem;
	UIEffectsSystemMaker uiEffectSystem;

	//inventorySystem
	InventoryMaker inventorySystem;
	BuildSystemMaker buildSystem;

	//grids
	GridList<StaticObject> staticGrid;
	GridList<DynamicObject> dynamicGrid;
	std::vector<spriteChainElement> visibleBackground, visibleTerrain;
	std::vector<WorldObject*> localTerrain;

	//test
	std::vector<std::pair<Tag, int>>* testInv = new std::vector<std::pair<Tag, int>>({ {Tag::chamomile, 2}, {Tag::chamomile, 2}, {Tag::chamomile, 2} });
};

#endif