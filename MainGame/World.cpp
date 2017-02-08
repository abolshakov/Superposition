#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "World.h"
#include "Deerchant.h"
#include "RoseTree.h"
#include "Helper.h"

using namespace sf;

World::World(int width, int height) : focusedObject(nullptr)
{
	auto blockSize = initSpriteMap();
	scaleFactor = getScaleFactor();

	this->width = width;
	this->height = height;

	//scaleSpriteMap(factor);

	blockSize.x = blockSize.x;
	blockSize.y = blockSize.y;

	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize);
}

bool cmpImgDraw(const WorldObject* first, const WorldObject* second)
{
	if (first->getPosition().y == second->getPosition().y)
		return first->getPosition().x < second->getPosition().x;
	return first->getPosition().y < second->getPosition().y;
}

Vector2i World::initSpriteMap()
{
	std::ifstream fin("World/objects.txt");

	int objectsNumber;
	fin >> objectsNumber;

	Vector2i maxSize;

	for (auto i = 0; i < objectsNumber; i++)
	{
		std::string name;
		fin >> name;
		spriteMap.insert({ name, boardSprite{} });
		auto sprite = &spriteMap[name];
		sprite->texture.loadFromFile("World/" + name);
		sprite->sprite.setTexture(sprite->texture);

		auto size = Vector2i(sprite->texture.getSize());
		if (size.x > maxSize.x)
			maxSize.x = size.x;

		if (size.y > maxSize.y)
			maxSize.y = size.y;
	}
	fin.close();

	return maxSize;
}

float World::getScaleFactor()
{
	auto heroHeight = Vector2i(spriteMap[heroTextureName].texture.getSize()).y;
	auto screenHeight = Helper::GetScreenSize().y;
	auto ratio = heroHeight / float(screenHeight);

	return heroToScreenRatio / ratio;
}

void World::generate(int objCount)
{
	auto s = float(sqrt(objCount));

	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{
			auto roseTreeType = rand() % 3 + 1;
			auto nameOfImage = "roseTree" + std::to_string(roseTreeType) + ".png";

			auto name = "roseTree" + std::to_string(i * int(ceil(s)) + j);
			auto position = Vector2f(i * (width / s), j * (height / s));

			staticGrid.addItem(new RoseTree(name, position), name, int(position.x), int(position.y));

			auto tree = staticGrid.getItemByName(name);
			tree->setTypeOfImage(std::to_string(roseTreeType));

			auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
			auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
			tree->setTextureSize(textureSize);
		}
	}

	auto heroPosition = Vector2f(0, 0);
	dynamicGrid.addItem(new Deerchant(heroName, heroPosition), heroName, int(heroPosition.x), int(heroPosition.y));

	focusedObject = dynamicGrid.getItemByName(heroName);

	auto textureBounds = spriteMap[heroTextureName].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));

	focusedObject->setTextureSize(textureSize);
}

Vector2f World::newSlippingPosition(DynamicObject *dynamicItem, TerrainObject *terrain, Vector2f newPosition, long long elapsedTime)
{
	Vector2f motion;
	if (dynamicItem->direction == LEFT || dynamicItem->direction == RIGHT)
	{
		if (newPosition.y <= terrain->getPosition().y)
		{
			motion = Vector2f(0, -dynamicItem->speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(0, dynamicItem->speed * elapsedTime);
		return motion;
	}
	if (dynamicItem->direction == UP || dynamicItem->direction == DOWN)
	{
		if (newPosition.x <= terrain->getPosition().x)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == UPLEFT)
	{
		if (newPosition.x <= terrain->getPosition().x)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		if (newPosition.x <= (terrain->getFocus2().x + terrain->getPosition().x) * 3 / 4)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, dynamicItem->speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(dynamicItem->speed * 2 * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == DOWNLEFT)
	{
		if (newPosition.x <= terrain->getPosition().x)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		if (newPosition.x <= (terrain->getFocus2().x + terrain->getPosition().x) * 3 / 4)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, -dynamicItem->speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(dynamicItem->speed * 2 * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == UPRIGHT)
	{
		if (newPosition.x >= terrain->getPosition().x)
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		if (newPosition.x >= (terrain->getFocus1().x + terrain->getPosition().x) / 2)
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime, dynamicItem->speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-dynamicItem->speed * 2 * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == DOWNRIGHT)
	{
		if (newPosition.x >= terrain->getPosition().x)
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		if (newPosition.x >= (terrain->getFocus1().x + terrain->getPosition().x) / 2)
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime, -dynamicItem->speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-dynamicItem->speed * 2 * elapsedTime, 0);
		return motion;
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

	auto staticItems = staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y, width);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y, width);

	for (auto dynamicItem : dynamicItems)
	{
		if (dynamicItem->direction == STAND)
			continue;

		auto intersects = false;
		auto newPosition = move(*dynamicItem, elapsedTime);
		if (newPosition.x < 0)
			newPosition.x = width - abs(newPosition.x);
		if (newPosition.x > width)
			newPosition.x = int(newPosition.x) % width;

		for (auto staticItem : staticItems)
		{
			auto terrain = dynamic_cast<TerrainObject*>(staticItem);
			if (!terrain)
				continue;

			if (isIntersectTerrain(newPosition, *terrain))
			{
				auto motionAfterSlipping = newSlippingPosition(dynamicItem, terrain, newPosition, elapsedTime);
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

			if (isIntersectDynamic(newPosition, *otherDynamicItem))
			{
				intersects = true;
				break;
			}
		}
		if (intersects)
			continue;

		auto currentPosition = dynamicItem->getPosition();
		auto newX = int(newPosition.x);
		auto newY = int(newPosition.y);
		dynamicItem->setPosition(newPosition);

		if (newX != int(currentPosition.x) || newY != int(currentPosition.y))
			dynamicGrid.updateItemPosition(dynamicItem->getName(), newX, newY);
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	const auto extra = staticGrid.getBlockSize();

	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);
	auto characterPosition = focusedObject->getPosition();

	Vector2i worldUpperLeft(int(characterPosition.x - (screenCenter.x + extra.x) / scaleFactor), int(characterPosition.y - (screenCenter.y + extra.x) / scaleFactor));
	Vector2i worldBottomRight(int(characterPosition.x + (screenCenter.x + extra.x) / scaleFactor), int(characterPosition.y + (screenCenter.y + extra.x) / scaleFactor));

	auto staticItems = staticGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y, width);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y, width);
	auto visibleItems = std::vector<WorldObject*>(staticItems.begin(), staticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());

	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->getPosition();
		auto worldTextureOffset = worldItem->getTextureOffset();
		auto worldTextureSize = worldItem->getTextureSize();

		auto spriteLeft = float((worldItemPosition.x - characterPosition.x - worldTextureOffset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - characterPosition.y - worldTextureOffset.y) * scaleFactor + screenCenter.y);
		auto spriteRight = float(spriteLeft + worldTextureSize.x * scaleFactor);
		auto spriteBottom = float(spriteTop + worldTextureSize.y * scaleFactor);

		//if (spriteRight > 0 && spriteLeft < screenSize.x && spriteBottom > 0 && spriteTop < screenSize.y)
		//{
			auto sprite = (&spriteMap[worldItem->getSpriteName(elapsedTime)])->sprite;
			sprite.setPosition(Vector2f(spriteLeft, spriteTop));
			sprite.setScale(scaleFactor, scaleFactor);
			window.draw(sprite);
			sprite.setPosition(Vector2f((worldItemPosition.x - width - characterPosition.x - worldTextureOffset.x) * scaleFactor + screenCenter.x, spriteTop));
			window.draw(sprite);
			sprite.setPosition(Vector2f((worldItemPosition.x + width - characterPosition.x - worldTextureOffset.x) * scaleFactor + screenCenter.x, spriteTop));
			window.draw(sprite);
		//}

	}

	/*auto terrain = dynamic_cast<TerrainObject*>(worldItem);
	if (terrain)
	{
	auto rectangle0 = RectangleShape();
	rectangle0.setPosition(Vector2f(focusedObject->getPosition().x - characterPosition.x + screenCenter.x - 10, focusedObject->getPosition().y - characterPosition.y + screenCenter.y - 10));
	rectangle0.setOutlineColor(Color::Green);
	rectangle0.setFillColor(Color::Red);
	rectangle0.setSize(Vector2f(20, 20));
	window.draw(rectangle0);
	auto rectangle = RectangleShape();
	rectangle.setPosition(Vector2f(terrain->getFocus1().x - characterPosition.x + screenCenter.x - 10, terrain->getFocus1().y - characterPosition.y + screenCenter.y - 10));
	rectangle.setOutlineColor(Color::Red);
	rectangle.setFillColor(Color::Red);
	rectangle.setSize(Vector2f(20, 20));
	window.draw(rectangle);
	auto rectangle2 = RectangleShape();
	rectangle2.setPosition(Vector2f(terrain->getFocus2().x - characterPosition.x + screenCenter.x - 10, terrain->getFocus2().y - characterPosition.y + screenCenter.y - 10));
	rectangle2.setOutlineColor(Color::Red);
	rectangle2.setFillColor(Color::Red);
	rectangle2.setSize(Vector2f(20, 20));
	window.draw(rectangle2);
	}*/

	/*auto rectangle3 = RectangleShape();
	rectangle3.setSize(Vector2f(float(width), float(height)));
	rectangle3.setOutlineColor(Color::Green);
	rectangle3.setFillColor(Color::Transparent);
	rectangle3.setOutlineThickness(2);
	rectangle3.setPosition(0 - characterPosition.x + screenCenter.x, 0 - characterPosition.y + screenCenter.y);
	window.draw(rectangle3);*/

	/*auto blockSize = staticGrid.getBlockSize();
	for (auto x = 0; x < width; x += blockSize)
	for (auto y = 0; y < height; y += blockSize)
	{
	auto rectangle4 = RectangleShape();
	rectangle4.setSize(Vector2f(float(blockSize), float(blockSize)));
	rectangle4.setOutlineColor(Color::Blue);
	rectangle4.setFillColor(Color::Transparent);
	rectangle4.setOutlineThickness(1);
	rectangle4.setPosition(x - characterPosition.x + screenCenter.x, y - characterPosition.y + screenCenter.y);
	window.draw(rectangle4);
	}*/
}

Vector2f World::move(const DynamicObject& dynamicObject, long long elapsedTime)
{
	auto angle = dynamicObject.direction * M_PI / 180;
	auto position = dynamicObject.getPosition();

	position.x = float(position.x + dynamicObject.speed * cos(angle) * elapsedTime);
	position.y = float(position.y - dynamicObject.speed * sin(angle) * elapsedTime);

	return position;
}

bool World::isIntersectTerrain(Vector2f position, const TerrainObject& terrain) const
{
	auto f1 = terrain.getFocus1();
	auto f2 = terrain.getFocus2();
	return sqrt((position.x - f1.x)*(position.x - f1.x) + (position.y - f1.y)*(position.y - f1.y)) + sqrt((position.x - f2.x)*(position.x - f2.x) + (position.y - f2.y)*(position.y - f2.y)) <= terrain.getEllipseSize();
}

bool World::isIntersectDynamic(Vector2f position, const DynamicObject& dynamic) const
{
	auto f1 = dynamic.getFocus1();
	auto f2 = dynamic.getFocus2();
	return sqrt((position.x - f1.x)*(position.x - f1.x) + (position.y - f1.y)*(position.y - f1.y)) + sqrt((position.x - f2.x)*(position.x - f2.x) + (position.y - f2.y)*(position.y - f2.y)) <= dynamic.getEllipseSize();
}

