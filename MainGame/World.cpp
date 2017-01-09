#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

#include "World.h"

using namespace sf;

World::World(int width, int height): screenSizeX(0), screenSizeY(0)
{
	this->width = width;
	this->height = height;
	auto blockSize = int(sqrt(min(width, height)));
	staticGrid = GridList<StaticObject>(width, height, blockSize);
	dynamicGrid = GridList<DynamicObject>(width, height, blockSize);
}

bool cmpImgDraw(const WorldObject* first, const WorldObject* second)
{
	return first->getPosition().y + first->getSize().y < second->getPosition().y + second->getSize().y;
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

void World::worldGenerate(int objCount)
{
	auto s = int(sqrt(objCount));
	
	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{
			auto position = Vector2i(i * (width / s), j * (height / s));
			staticGrid.addItem(new Stone(Vector2f(position), spriteMap["stone.png"].texture.getSize(), "stone.png", "obj"),"stone"+std::to_string(i * s+j), position.x, position.y);
		}
	}
}
void World::objectsInteract(DynamicObject heroSprite)
{

}

void World::drawBoard(RenderWindow *window, Vector2f screenSize, DynamicObject* focusedObject)
{
	const auto extra = 600;
	auto characterSize = focusedObject->getSize();

	Vector2f characterScreen;
	characterScreen.x = screenSize.x / 2 - characterSize.x / 2;
	characterScreen.y = screenSize.y / 2 - characterSize.y / 2;

	auto characterPosition = focusedObject->getPosition();
	Vector2f characterScreenToWorld;
	characterScreenToWorld.x = characterPosition.x + characterSize.x / 2;
	characterScreenToWorld.y = characterPosition.y + characterSize.y / 2;

	Vector2i worldUpperLeft;
	worldUpperLeft.x = int(characterScreenToWorld.x - screenSize.x / 2);
	worldUpperLeft.y = int(characterScreenToWorld.y - screenSize.y / 2);
	Vector2i worldBottomRight;
	worldBottomRight.x = int(characterScreenToWorld.x + screenSize.x / 2);
	worldBottomRight.y = int(characterScreenToWorld.y + screenSize.y / 2);

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto visibleItems = std::vector<WorldObject*>(staticItems.begin(), staticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());
	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());

	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (auto i = 0; i < visibleItems.size(); i++)
	{
		auto worldItem = visibleItems[i];
		auto spriteItem = &spriteMap[worldItem->getName()];

		auto spriteX = worldItem->getPosition().x - characterPosition.x + characterScreen.x;
		auto spriteY = worldItem->getPosition().y - characterPosition.y + characterScreen.y;
		spriteItem->sprite.setPosition(Vector2f(spriteX, spriteY));
		window->draw(spriteItem->sprite);
	}
}

