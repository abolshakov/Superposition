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
	auto blockSize = 1500;
	staticGrid = GridList<StaticObject>(width, height, blockSize);
	
	dynamicGrid = GridList<DynamicObject>(width, height, blockSize);
}

bool cmpImgDraw(const WorldObject* first, const WorldObject* second)
{
	return first->getPosition().y < second->getPosition().y;
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
	srand(time(0));
	auto s = int(sqrt(objCount));

	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{
			int roseTreeType = rand() % 3 + 1;
			std::string nameOfImage = "roseTree" + std::to_string(roseTreeType);
			nameOfImage += ".png";
			auto textureSize = Vector2f(spriteMap[nameOfImage].texture.getSize());
			auto name = "roseTree" + std::to_string(i * s + j);
			auto position = Vector2f(i * (width / s), j * (height / s));
			staticGrid.addItem(new RoseTree(name, position), name, position.x, position.y);
			auto tree = staticGrid.getItemByName(name);
			tree->setTypeOfImage(std::to_string(roseTreeType));
			tree->setTextureSize(textureSize);
		}
	}

	std::string heroName = "hero";
	auto heroTextureSize = Vector2f(spriteMap["heroF_0.png"].texture.getSize());
	auto heroPosition = Vector2f(2000, 2000);
	dynamicGrid.addItem(new Deerchant(heroName, heroPosition), heroName, int(heroPosition.x), int(heroPosition.y));

	focusedObject = dynamicGrid.getItemByName(heroName);
	focusedObject->setTextureSize(heroTextureSize);
}
Vector2f World::newSlippingPosition(DynamicObject *dynamicItem, TerrainObject *terrain, Vector2f newPosition, float elapsedTime)
{
	Vector2f motion;
	if (dynamicItem->direction == LEFT || dynamicItem->direction == RIGHT)
	{
		if (newPosition.y <= terrain->getPosition().y)
		{
			motion = Vector2f(0, -dynamicItem->speed * elapsedTime);
			return motion;
		}
		else
		{
			motion = Vector2f(0, dynamicItem->speed * elapsedTime);
			return motion;
		}
	}
	else
		if (dynamicItem->direction == UP || dynamicItem->direction == DOWN)
		{
			if (newPosition.x <= terrain->getPosition().x)
			{
				motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
				return motion;
			}
			else
			{
				motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
				return motion;
			}
		}
		else
			if (dynamicItem->direction == UPLEFT)
			{
				if (newPosition.x <= terrain->getPosition().x)
				{
					motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
					return motion;
				}
				else
				{
					if (newPosition.x <= (terrain->getFocus2().x + terrain->getPosition().x) * 3 / 4)
					{
						motion = Vector2f(-dynamicItem->speed * elapsedTime, dynamicItem->speed * elapsedTime);
						return motion;
					}
					else
					{
						motion = Vector2f(dynamicItem->speed * 2 * elapsedTime, 0);
						return motion;
					}
				}
			}
			else
				if (dynamicItem->direction == DOWNLEFT)
				{
					if (newPosition.x <= terrain->getPosition().x)
					{
						motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
						return motion;
					}
					else
					{
						if (newPosition.x <= (terrain->getFocus2().x + terrain->getPosition().x) * 3 / 4)
						{
							motion = Vector2f(-dynamicItem->speed * elapsedTime, -dynamicItem->speed * elapsedTime);
							return motion;
						}
						else
						{
							motion = Vector2f(dynamicItem->speed * 2 * elapsedTime, 0);
							return motion;
						}
					}
				}
				else
					if (dynamicItem->direction == UPRIGHT)
					{
						if (newPosition.x >= terrain->getPosition().x)
						{
							motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
							return motion;
						}
						else
						{
							if (newPosition.x >= (terrain->getFocus1().x + terrain->getPosition().x) / 2)
							{
								motion = Vector2f(dynamicItem->speed * elapsedTime, dynamicItem->speed * elapsedTime);
								return motion;
							}
							else
							{
								motion = Vector2f(-dynamicItem->speed * 2 * elapsedTime, 0);
								return motion;
							}
						}
					}
					else
						if (dynamicItem->direction == DOWNRIGHT)
						{
							if (newPosition.x >= terrain->getPosition().x)
							{
								motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
								return motion;
							}
							else
							{
								if (newPosition.x >= (terrain->getFocus1().x + terrain->getPosition().x) / 2)
								{
									motion = Vector2f(dynamicItem->speed * elapsedTime, -dynamicItem->speed * elapsedTime);
									return motion;
								}
								else
								{
									motion = Vector2f(-dynamicItem->speed * 2 * elapsedTime, 0);
									return motion;
								}
							}
						}
	return Vector2f(-1000000, -1000000);
}
void World::interact(RenderWindow& window, long long elapsedTime)
{
	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto characterPosition = focusedObject->getPosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenSize.x / 2), int(characterPosition.y - screenSize.y / 2));
	Vector2i worldBottomRight(int(characterPosition.x + screenSize.x / 2), int(characterPosition.y + screenSize.y / 2));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	for (auto dynamicItem : dynamicItems)
	{
		if (dynamicItem->direction == STAND)
			continue;

		bool intersects = false;
		auto newPosition = move(*dynamicItem, elapsedTime);

		for (auto staticItem : staticItems)
		{
			TerrainObject* terrain = dynamic_cast<TerrainObject*>(staticItem);
			if (!terrain)
				continue;

			if (isIntersectTerrain(newPosition, *dynamicItem, *terrain))
			{
				Vector2f motionAfterSlipping = newSlippingPosition(dynamicItem, terrain, newPosition, elapsedTime);
				if (motionAfterSlipping.x != -1000000 && motionAfterSlipping.y != -1000000)
				{
					newPosition = Vector2f(dynamicItem->getPosition().x + motionAfterSlipping.x, dynamicItem->getPosition().y + motionAfterSlipping.y);
					continue;
				}
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
			if (isIntersectDynamic(newPosition, *dynamicItem, *otherDynamicItem))
			{
				intersects = true;
				break;
			}
		}
		if (!intersects)
		{
			dynamicItem->setPosition(newPosition);
			dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);
		}
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);
	auto characterPosition = focusedObject->getPosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenCenter.x), int(characterPosition.y - screenCenter.y));
	Vector2i worldBottomRight(int(characterPosition.x + screenCenter.x), int(characterPosition.y + screenCenter.y));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra, worldUpperLeft.y - extra, worldBottomRight.x + extra, worldBottomRight.y + extra);
	auto visibleItems = std::vector<WorldObject*>(staticItems.begin(), staticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());

	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);
	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->getPosition();
		auto worldTextureOffset = worldItem->getTextureOffset();
		auto spriteItem = &spriteMap[worldItem->getSpriteName(elapsedTime)];

		auto spriteX = float(worldItemPosition.x - characterPosition.x + screenCenter.x - worldTextureOffset.x);
		auto spriteY = float(worldItemPosition.y - characterPosition.y + screenCenter.y - worldTextureOffset.y);
		spriteItem->sprite.setPosition(Vector2f(spriteX, spriteY));

		window.draw(spriteItem->sprite);

		TerrainObject* terrain = dynamic_cast<TerrainObject*>(worldItem);
		if (terrain)
		{
			auto rectangle0 = sf::RectangleShape();
			rectangle0.setPosition(sf::Vector2f(focusedObject->getPosition().x - characterPosition.x + screenCenter.x - 10, focusedObject->getPosition().y - characterPosition.y + screenCenter.y - 10));
			rectangle0.setOutlineColor(sf::Color::Green);
			rectangle0.setFillColor(sf::Color::Red);
			rectangle0.setSize(Vector2f(20, 20));
			window.draw(rectangle0);
			auto rectangle = sf::RectangleShape();
			rectangle.setPosition(sf::Vector2f(terrain->getFocus1().x - characterPosition.x + screenCenter.x - 10, terrain->getFocus1().y - characterPosition.y + screenCenter.y - 10));
			rectangle.setOutlineColor(sf::Color::Red);
			rectangle.setFillColor(sf::Color::Red);
			rectangle.setSize(Vector2f(20, 20));
			window.draw(rectangle);
			auto rectangle2 = sf::RectangleShape();
			rectangle2.setPosition(sf::Vector2f(terrain->getFocus2().x - characterPosition.x + screenCenter.x - 10, terrain->getFocus2().y - characterPosition.y + screenCenter.y - 10));
			rectangle2.setOutlineColor(sf::Color::Red);
			rectangle2.setFillColor(sf::Color::Red);
			rectangle2.setSize(Vector2f(20, 20));
			window.draw(rectangle2);
		}
	}
	auto rectangle3 = sf::RectangleShape();
	rectangle3.setSize(sf::Vector2f(width, height));
	rectangle3.setOutlineColor(sf::Color::Green);
	rectangle3.setFillColor(sf::Color::Transparent);
	rectangle3.setOutlineThickness(2);
	rectangle3.setPosition(0 - characterPosition.x + screenCenter.x, 0 - characterPosition.y + screenCenter.y);
	window.draw(rectangle3);

	auto blockSize = staticGrid.getBlockSize();
	for (auto x = 0; x < width; x += blockSize)
		for (auto y = 0; y < height; y += blockSize)
		{
			auto rectangle4 = sf::RectangleShape();
			rectangle4.setSize(sf::Vector2f(blockSize, blockSize));
			rectangle4.setOutlineColor(sf::Color::Blue);
			rectangle4.setFillColor(sf::Color::Transparent);
			rectangle4.setOutlineThickness(1);
			rectangle4.setPosition(x - characterPosition.x + screenCenter.x, y - characterPosition.y + screenCenter.y);
			window.draw(rectangle4);
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

bool World::isIntersectTerrain(Vector2f position, const DynamicObject& dynamic, const TerrainObject& other)
{
	Vector2f dynPos = position;
	Vector2f f1 = other.getFocus1();
	Vector2f f2 = other.getFocus2();
	return (sqrt((dynPos.x - f1.x)*(dynPos.x - f1.x) + (dynPos.y - f1.y)*(dynPos.y - f1.y)) + sqrt((dynPos.x - f2.x)*(dynPos.x - f2.x) + (dynPos.y - f2.y)*(dynPos.y - f2.y)) <= other.getEllipseSize());
}

bool World::isIntersectDynamic(Vector2f position, const DynamicObject& dynamic, const DynamicObject& other)
{
	Vector2f dynPos = position;
	Vector2f f1 = other.getFocus1();
	Vector2f f2 = other.getFocus2();
	return (sqrt((dynPos.x - f1.x)*(dynPos.x - f1.x) + (dynPos.y - f1.y)*(dynPos.y - f1.y)) + sqrt((dynPos.x - f2.x)*(dynPos.x - f2.x) + (dynPos.y - f2.y)*(dynPos.y - f2.y)) <= other.getEllipseSize());
}

