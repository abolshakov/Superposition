#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>
#include <stack>

using namespace sf;
// mobs 101 - 199; craft objects 201 - 299; world objects 301 - 399; flowers 401 - 499; auxiliary objects 501 - 599;
enum class Tag { 
	hero1 = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109,
	heroBag = 201, noose = 202, hareTrap = 216, fence = 218, inkyBlackPen = 219, tree = 301, grass = 302, spawn = 303, bonefireOfInsight = 304, homeCosiness = 305,
	mushroomStone = 306, mushroomsOnStone = 307, ground = 311, groundConnection = 312, brazier = 314, rock = 317, 
	stump = 319, droppedLoot = 320, fog = 350,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405, buildObject = 501, dropPoint = 502, emptyDraft = 503, emptyPage = 504, emptyCell = 505, selectedCell = 506
};

enum State { common = 1, absorbed = 2, constructed = 3 };

struct spriteChainElement
{
	std::string path;
	Vector2f offset;
    Vector2f position;
	Vector2f size;
    Vector2f scaleRatio = {0, 0};
	float rotation = 0;
    float transparency = 100;
	int animationLength = 1;
	int zCoord = 1;
    bool isBackground = false;
	Tag tag;
};

struct birthStaticInfo
{
	Tag tag;
	Vector2f position;
	int typeOfObject = 1;
	int count = 1;
	std::vector<std::pair<Tag, int>> inventory;
};

struct  birthDynamicInfo
{
	Tag tag;
	Vector2f position;
};

class WorldObject
{
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();
	int getZCoords() { return zCoord; }
	int getAnimationLength() { return animationLength; }
	int getType() { return typeOfObject; }
	int getVarietyOfTypes() { return varietyOfTypes; }
	bool getDeletePromise() { return deletePromise; }
	int getRadius() { return radius; }
	std::string getToSaveName() { return toSaveName; }
	std::string getName() const { return name; }
	virtual void prepareSpriteNames(long long elapsedTime) = 0;
	virtual int getSpriteNumber() = 0;
	Vector2f getPosition() const { return position; }
	Vector2f *getPtrPosition() { return &position; }
	Vector2i getTextureSize() const { return  Vector2i(textureBox.width, textureBox.height); }
	Vector2i getTextureOffset() const { return Vector2i(textureBoxOffset.x, textureBoxOffset.y); }
	Vector2f getScaleRatio();
	Vector2i getConditionalSizeUnits() { return conditionalSizeUnits; }
	virtual Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) = 0;
	virtual int getBuildType(Vector2f ounPos, Vector2f otherPos) = 0;
	IntRect getOriginalTextureBox() { return originalTextureBox; }
	State getState() { return state; }	
	std::pair<std::stack<birthStaticInfo>, std::stack<birthDynamicInfo>> getBirthObjects() { return std::make_pair(birthStatics, birthDynamics); }

	void clearBirthStack() { birthStatics = std::stack<birthStaticInfo>(); birthDynamics = std::stack<birthDynamicInfo>(); }
	void setPosition(Vector2f newPosition);
	void setName(std::string name) { this->name = name; }
	void deletePromiseOn() { deletePromise = true; }
	virtual void setTextureSize(Vector2f textureSize);
	void setState(State state) { this->state = state; }

	bool isTransparent = false, isVisibleName = false, isSelected = false;
	bool isProcessed = false;
	bool isBackground = false, isTerrain = false, isDotsAdjusted = false, isMultiellipse = false, intangible = false;

	virtual Vector2i calculateTextureOffset();
	virtual void initPedestal();

	int transparency = 100;
	int currentBlock = 0;
	std::vector<std::pair<Tag, int>> inventory;

	Tag tag;

    std::vector<spriteChainElement> additionalSprites;
protected:
	int typeOfObject{};
	int animationLength{};
	int zCoord = 1;
	std::vector<int> currentSprite;
	int varietyOfTypes{};
	bool deletePromise = false;
	float timeForNewSprite{}, animationSpeed{};
	std::string name, toSaveName;
	IntRect textureBox, originalTextureBox;
	Vector2i textureBoxOffset;
	Vector2i conditionalSizeUnits;
	Vector2f position;
	int radius{};
	State state = common;
	std::stack<birthStaticInfo> birthStatics;
	std::stack<birthDynamicInfo> birthDynamics;
};

#endif