#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "World.h"
#include "Deerchant.h"

using namespace sf;

World::World(int width, int height) : screenSizeX(0), screenSizeY(0), focusedObject(nullptr)
{
	this->width = width;
	this->height = height;
	auto blockSize = int(sqrt(min(width, height)));
	staticGrid = GridList<StaticObject>(width, height, blockSize);
	dynamicGrid = GridList<DynamicObject>(width, height, blockSize);
}

bool cmpImgDraw(const WorldObject* first, const WorldObject* second)
{
	return first->getPosition().y + first->getTextureSize().y < second->getPosition().y + second->getTextureSize().y;
}

void World::initSpriteMap()
{
	std::ifstream fin("World/objects.txt");

	int objectsNumber;
	fin >> objectsNumber;

	for (auto i = 0; i < objectsNumber; i++)
	{
		std::string name;
		fin >> name;
		spriteMap.insert({ name, boardSprite{} });
		auto sprite = &spriteMap[name];
		fin >> sprite->type;
		sprite->texture.loadFromFile("World/" + name);
		sprite->sprite.setTexture(sprite->texture);
	}
	fin.close();
}

void World::generate(int objCount)
{
	auto s = int(sqrt(objCount));

	auto stoneTextureSize = Vector2i(spriteMap["stone.png"].texture.getSize());
	auto stoneSize = Vector2f(stoneTextureSize.x, stoneTextureSize.y / 2);
	auto stoneTextureOffset = Vector2i(0, stoneTextureSize.y - stoneSize.y);

	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{
			auto name = "stone" + std::to_string(i * s + j);
			auto position = Vector2f(i * (width / s), j * (height / s));
			staticGrid.addItem(new Stone(name, FloatRect(position, stoneSize), IntRect(Vector2i(position.x - stoneTextureOffset.x, position.y - stoneTextureOffset.y), stoneTextureSize)), name, position.x, position.y);
		}
	}

	std::string heroName = "hero";
	auto heroTextureSize = Vector2i(spriteMap["heroF_0.png"].texture.getSize());
	auto heroSize = Vector2f(heroTextureSize.x, heroTextureSize.y / 5);
	auto heroTextureOffset = Vector2i(0, heroTextureSize.y - heroSize.y);
	auto heroPosition = Vector2f(400, 400);
	dynamicGrid.addItem(new Deerchant(heroName, FloatRect(heroPosition, heroSize), IntRect(Vector2i(heroPosition.x - heroTextureOffset.x, heroPosition.y - heroTextureOffset.y), heroTextureSize)), heroName, int(heroPosition.x), int(heroPosition.y));

	focusedObject = dynamicGrid.getItemByName(heroName);
}

void World::interact(RenderWindow& window, long long elapsedTime)
{
	const auto extra = 600;
	auto screenSize = window.getSize();
	auto characterSize = focusedObject->getTextureSize();
	auto characterPosition = focusedObject->getTexturePosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenSize.x / 2 + characterSize.x / 2), int(characterPosition.y - screenSize.y / 2 + characterSize.y / 2));
	Vector2i worldBottomRight(int(characterPosition.x + screenSize.x / 2 - characterSize.x / 2), int(characterPosition.y + screenSize.y / 2 - characterSize.y / 2));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);

	for (auto dynamicItem : dynamicItems)
	{
		bool intersects = false;
		if (dynamicItem->direction != STAND)
		{
			auto newPosition = move(*dynamicItem, elapsedTime);
			for (auto staticItem : staticItems)
			{
				if (isIntersect(newPosition, *dynamicItem, *staticItem))
				{
					intersects = true;
					break;
				}
			}
			if (intersects)
				continue;
			for (auto otherDynamicItem : dynamicItems)
			{
				if (otherDynamicItem == dynamicItem)
					continue;
				if (isIntersect(newPosition, *dynamicItem, *otherDynamicItem))
				{
					intersects = true;
					break;
				}
			}
			if (!intersects)
				dynamicItem->setPosition(newPosition);
		}
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	const auto extra = 600;
	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);
	auto characterSize = focusedObject->getTextureSize();
	auto characterHalfSize = Vector2i(characterSize.x / 2, characterSize.y / 2);
	auto characterPosition = focusedObject->getTexturePosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenCenter.x + characterHalfSize.x), int(characterPosition.y - screenCenter.y + characterHalfSize.y));
	Vector2i worldBottomRight(int(characterPosition.x + screenCenter.x - characterHalfSize.x), int(characterPosition.y + screenCenter.y - characterHalfSize.y));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto visibleItems = std::vector<WorldObject*>(staticItems.begin(), staticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());

	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->getTexturePosition();
		auto spriteItem = &spriteMap[worldItem->getSpriteName(elapsedTime)];

		auto spriteX = float(worldItemPosition.x - characterPosition.x + screenCenter.x - characterHalfSize.x);
		auto spriteY = float(worldItemPosition.y - characterPosition.y + screenCenter.y - characterHalfSize.y);
		spriteItem->sprite.setPosition(Vector2f(spriteX, spriteY));

		window.draw(spriteItem->sprite);
	}
}

Vector2f World::move(const DynamicObject& dynamicObject, long long elapsedTime)
{
	auto angle = dynamicObject.direction * M_PI / 180;
	auto position = dynamicObject.getPosition();

	position.x = float(position.x + dynamicObject.speed * cos(angle) * elapsedTime);
	position.y = float(position.y - dynamicObject.speed * sin(angle) * elapsedTime);

	return position;
}

bool World::isIntersect(Vector2f position, const DynamicObject& dynamic, const WorldObject& other)
{ 
	return false;
	Rect<float> first(position.x, position.y, dynamic.getSize().x, dynamic.getSize().y);
	Rect<float> second = other.getBoundingBox();
	return (first.left <= second.left && first.left + first.width >= second.left || second.left <= first.left && second.left + second.width >= first.left)
		&& (first.top <= second.top && first.top + first.height >= second.top || second.top <= first.top && second.top + second.height >= first.top);
	//return dynamic.getBoundingBox().intersects(other.getBoundingBox());
}

