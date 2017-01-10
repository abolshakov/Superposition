#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

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
	return first->position.y + first->getSize().y < second->position.y + second->getSize().y;
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
	auto size = spriteMap["stone.png"].texture.getSize();

	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{
			auto position = Vector2i(i * (width / s), j * (height / s));
			auto name = "stone" + std::to_string(i * s + j);
			staticGrid.addItem(new Stone(Vector2f(position), size, name), name, position.x, position.y);
		}
	}
	auto heroPosition = Vector2f(100, 100);
	std::string heroName = "hero";
	dynamicGrid.addItem(new Deerchant(heroPosition, spriteMap["heroF_0.png"].texture.getSize(), heroName), heroName, int(heroPosition.x), int(heroPosition.y));
	
	focusedObject = dynamicGrid.getItemByName(heroName);
}

void World::interact(long long elapsedTime) const
{
	auto newPosition = move(*focusedObject, elapsedTime);
	focusedObject->position = newPosition;
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	const auto extra = 600;
	auto screenSize = window.getSize();
	auto characterSize = focusedObject->getSize();
	auto characterPosition = focusedObject->position;

	Vector2u characterScreen(screenSize.x / 2 - characterSize.x / 2, screenSize.y / 2 - characterSize.y / 2);
	Vector2f characterScreenToWorld(characterPosition.x + characterSize.x / 2, characterPosition.y + characterSize.y / 2);
	Vector2i worldUpperLeft(int(characterScreenToWorld.x - screenSize.x / 2), int(characterScreenToWorld.y - screenSize.y / 2));
	Vector2i worldBottomRight(int(characterScreenToWorld.x + screenSize.x / 2), int(characterScreenToWorld.y + screenSize.y / 2));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto visibleItems = std::vector<WorldObject*>(staticItems.begin(), staticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());

	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->position;
		auto spriteItem = &spriteMap[worldItem->getSpriteName(elapsedTime)];

		auto spriteX = worldItemPosition.x - characterPosition.x + characterScreen.x;
		auto spriteY = worldItemPosition.y - characterPosition.y + characterScreen.y;
		spriteItem->sprite.setPosition(Vector2f(spriteX, spriteY));

		window.draw(spriteItem->sprite);
	}
}

Vector2f World::move(const DynamicObject& dynamicObject, long long elapsedTime)
{
	auto speed = dynamicObject.speed;
	auto direction = dynamicObject.direction;
	auto position = dynamicObject.position;

	auto distance = speed * elapsedTime;
	auto diagonalDistance = speed * 0.7f * elapsedTime;

	switch (direction)
	{
	case LEFT:
		position.x -= distance;
		break;
	case RIGHT:
		position.x += distance;
		break;
	case UP:
		position.y -= distance;
		break;
	case DOWN:
		position.y += distance;
		break;
	case UPLEFT:
		position.x -= diagonalDistance;
		position.y -= diagonalDistance;
		break;
	case UPRIGHT:
		position.x += diagonalDistance;
		position.y -= diagonalDistance;
		break;
	case DOWNLEFT:
		position.x -= diagonalDistance;
		position.y += diagonalDistance;
		break;
	case DOWNRIGHT:
		position.x += diagonalDistance;
		position.y += diagonalDistance;
		break;
	case STAND:
		break;
	default:;
	}
	return position;
}

