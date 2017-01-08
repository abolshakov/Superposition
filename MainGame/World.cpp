#include <windows.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include "World.h"

using namespace sf;
using namespace std;

World::World(int width, int height)
{
	this->width = width;
	this->height = height;
	staticObjects = GridList<StaticObject>(width, height, int(sqrt(min(width, height))));
	dynamicObjects = GridList<DynamicObject>(width, height, int(sqrt(min(width, height))));
}

bool cmpImgDraw(WorldObject obj1, WorldObject obj2)
{
	return obj1.getPosition().y + obj1.getSize().y < obj2.getPosition().y + obj2.getSize().y;
}

void World::initSpriteMap()
{
	ifstream fin("World/objects.txt");
	fin >> objectsNumber;
	for (int i = 0; i < objectsNumber; i++)
	{
		string name;
		fin >> name;
		spriteMap.insert({ name, boardSprite{} });
		boardSprite *sprite = &spriteMap[name];
		fin >> sprite->type;
		sprite->texture.loadFromFile("World/" + name);
		sprite->sprite.setTexture(sprite->texture);
	}
	fin.close();
}

void World::worldGenerate(int objCount)
{

	//srand(time(0));
	int s = sqrt(objCount);
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			Stone item(Vector2f(i * (width / s), j * (height / s)), spriteMap["stone.png"].texture.getSize(), "stone.png", "obj");
			staticObjects.addItem(item, item.getPosition().x, item.getPosition().y);
		}
	}
}
void World::objectsInteract(DynamicObject heroSprite)
{

}

void World::drawBoard(RenderWindow *window, Vector2f screenSize, DynamicObject heroSprite)
{
	const int extra = 600;
	Vector2u characterSize = heroSprite.getSize();

	Vector2f characterScreen;
	characterScreen.x = screenSize.x / 2 - characterSize.x / 2;
	characterScreen.y = screenSize.y / 2 - characterSize.y / 2;

	Vector2f characterPosition = heroSprite.getPosition();
	Vector2f characterScreenToWorld;
	characterScreenToWorld.x = characterPosition.x + characterSize.x / 2;
	characterScreenToWorld.y = characterPosition.y + characterSize.y / 2;

	Vector2f worldUpperLeft;
	worldUpperLeft.x = characterScreenToWorld.x - screenSize.x / 2;
	worldUpperLeft.y = characterScreenToWorld.y - screenSize.y / 2;
	Vector2f worldBottomRight;
	worldBottomRight.x = characterScreenToWorld.x + screenSize.x / 2;
	worldBottomRight.y = characterScreenToWorld.y + screenSize.y / 2;

	float characterBottom = characterPosition.y + characterSize.y;

	vector<StaticObject> staticItems = staticObjects.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	vector<DynamicObject> dynamicItems = dynamicObjects.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	vector<WorldObject> visibleItems = vector<WorldObject>(staticItems.begin(), staticItems.end());
	vector<WorldObject> visibleDynamicItems = vector<WorldObject>(dynamicItems.begin(), dynamicItems.end());
	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());

	curDraw = staticObjects.getIndexByPoint(characterPosition.x, characterPosition.y);
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (int i = 0; i < visibleItems.size(); i++)
	{
		WorldObject *worldItem = &visibleItems[i];
		boardSprite *spriteItem = &spriteMap[worldItem->getName()];

		float spriteX = worldItem->getPosition().x - characterPosition.x + characterScreen.x;
		float spriteY = worldItem->getPosition().y - characterPosition.y + characterScreen.y;
		spriteItem->sprite.setPosition(Vector2f(spriteX, spriteY));
		window->draw(spriteItem->sprite);
	}
}

