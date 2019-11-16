#pragma once
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>
#include <stack>
#include "UIEffectsSystemMaker.h"

class WorldObject;
using namespace sf;
// mobs 101 - 199; craft objects 201 - 299; world objects 301 - 399; flowers 401 - 499; auxiliary objects 501 - 599;
enum class Tag {
	hero1 = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109, nightmare1 = 110, nightmare2 = 111, nightmare3 = 112,
	heroBag = 201, noose = 202, totem = 211, hareTrap = 216, fence = 218, inkyBlackPen = 219, 
	unknownWreath = 251, hareWreath = 252, owlWreath = 253, tree = 301, grass = 302, spawn = 303, bonefireOfInsight = 304, homeCosiness = 305,
	mushroomStone = 306, mushroomsOnStone = 307, ground = 311, groundConnection = 312, brazier = 314, wreathTable = 315, rock = 317, 
	stump = 319, droppedLoot = 320, fog = 350,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405, buildObject = 501, dropPoint = 502, emptyDraft = 503, emptyPage = 504, emptyCell = 505, selectedCell = 506, clapWhirl = 507
};

enum State { common = 1, absorbed = 2, constructed = 3 };

struct spriteChainElement
{
	std::string path;
	Vector2f offset;
    Vector2f position = { 0,0 };
	Vector2f size;
    Vector2f scaleRatio = {0, 0};
	float rotation = 0;
	int animationLength = 1;
	int zCoord = 1;
    bool isBackground = false, mirrored = false, antiTransparent = false;
	Tag tag;
	Color color;
};

struct birthStaticInfo
{
	Tag tag;
	Vector2f position = { 0,0 };
	int typeOfObject = 1;
	int count = 1;
	std::vector<std::pair<Tag, int>> inventory;
};

struct  birthDynamicInfo
{
	Tag tag;
	Vector2f position = { 0,0 };
	WorldObject* owner = nullptr;
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
	float getHealthPoint() { return healthPoint; }
	float getMaxHealthPointValue() { return maxHealthPointValue; }
	bool getDeletePromise() { return deletePromise; }
	bool getMirroredState() { return mirrored; }
	int getRadius() { return radius; }
	std::string getToSaveName() { return toSaveName; }
	std::string getName() const { return name; }
	virtual void prepareSpriteNames(long long elapsedTime, float scaleFactor = 1) = 0;
	virtual void onSpriteChange();
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
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	void setName(std::string name) { this->name = name; }
	void deletePromiseOn() { deletePromise = true; }
	virtual void setTextureSize(Vector2f textureSize);
	void setState(State state) { this->state = state; }
	virtual void takeDamage(float damage, Vector2f attackerPos = { -1, -1 });

	bool isTransparent = false, isVisibleName = false, isSelected = false, isVisibleInventory = false;
	bool isProcessed = false;
	bool isBackground = false, isTerrain = false, isDotsAdjusted = false, isMultiellipse = false, intangible = false;

	virtual Vector2i calculateTextureOffset() = 0;
	virtual void initPedestal();

	Color color = Color(255, 255, 255);
	int currentBlock = 0;
	std::vector<std::pair<Tag, int>> inventory = {};

	Tag tag;

    std::vector<spriteChainElement> additionalSprites;
protected:
	int typeOfObject{};
	int animationLength{};
	int zCoord = 1;
	std::vector<int> currentSprite;
	int varietyOfTypes{};
	bool deletePromise = false;
	bool mirrored = false;
	float timeForNewSprite{}, animationSpeed{};
	float healthPoint = 0, armor = 1, maxHealthPointValue = 0;
	std::string name, toSaveName;
	IntRect textureBox, originalTextureBox;
	Vector2i textureBoxOffset;
	Vector2i conditionalSizeUnits;
	Vector2f position = { 0, 0 };
	int radius{};
	State state = common;
	std::stack<birthStaticInfo> birthStatics;
	std::stack<birthDynamicInfo> birthDynamics;
};

#endif