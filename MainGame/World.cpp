#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "World.h"
#include "DynamicObject.h"
#include "RoseTree.h"
#include "Grass.h"
#include "Helper.h"
#include "TerrainObject.h"
#include "Spawn.h"

using namespace sf;
float pi = 3.14159265358979323846;
World::World(int width, int height) : focusedObject(nullptr)
{
	timeAfterSave = 60000000;
	timeForNewSave = 0;
	blockSize = initSpriteMap();
	scaleFactor = getScaleFactor();

	this->width = width;
	this->height = height;

	//scaleSpriteMap(factor);

	blockSize.x = blockSize.x;
	blockSize.y = blockSize.y;
	initShaders();
}

void World::initShaders()
{
	if (!sf::Shader::isAvailable())
	{
		std::cerr << "Shader are not available" << std::endl;
	}

	// Load shaders
	
	if (!spiritWorldShader.loadFromFile("World/Shaders/water.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
	}	

	if (!distortionMap.loadFromFile("World/Shaders/noiseWater.png"))
	{
		sf::err() << "Error while loading distortion map" << std::endl;
	}
	// It is important to set repeated to true to enable scrolling upwards
	distortionMap.setRepeated(true);
	// Setting smooth to true lets us use small maps even on larger images
	distortionMap.setSmooth(true);

	// Set texture to the shader
	spiritWorldShader.setUniform("currentTexture", sf::Shader::CurrentTexture);
	spiritWorldShader.setUniform("distortionMapTexture", distortionMap);

	// Shader parameters
	float level = 0;
}

void World::setScaleFactor(int delta)
{
	if (delta == -1 && scaleFactor > 0.75)	
	{
		scaleFactor -= 0.01;
		scaleDecrease = -0.03;
	}
	else
		if (delta == 1 && scaleFactor < 1.5)
		{
			scaleFactor += 0.01;
			scaleDecrease = 0.03;
		}
}

void World::scaleSmoothing()
{
	if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
	{
		if (scaleFactor != 0.75 && scaleFactor != 1.5)
			scaleFactor += scaleDecrease;
		if (scaleDecrease < 0 && scaleFactor < 0.75)
			scaleFactor = 0.75;
		if (scaleDecrease > 0 && scaleFactor > 1.5)
			scaleFactor = 1.5;

		if (scaleDecrease < 0)
		{
			scaleDecrease += 0.001;
		}
		else
			if (scaleDecrease > 0)
			{
				scaleDecrease -= 0.001;
			}


		timeForScaleDecrease = 0;
	}
}

bool cmpImgDraw(WorldObject* first, WorldObject* second)
{
	if (first->isTerrain == true && second->isTerrain == false)
		return true;
	if (first->isTerrain == false && second->isTerrain == true)
		return false;
	if (first->getPosition().y == second->getPosition().y)
		return first->getPosition().x < second->getPosition().x;
	return first->getPosition().y < second->getPosition().y;
}

Vector2i World::initSpriteMap()
{
	std::ifstream fin(spriteNameFileDirectory);

	int objectsNumber;
	Vector2i maxSize;
	std::string name;
	while (fin >> name)
	{				
		spriteMap.insert({ name, boardSprite{} });
		auto sprite = &spriteMap[name];
		sprite->texture.loadFromFile("World/" + name);
		sprite->sprite.setTexture(sprite->texture);
		if (Helper::getObjectName(Helper::withoutNums(name)) == "shader")
		{
			sprite->sprite.setScale(Helper::GetScreenSize().x / sprite->texture.getSize().x, Helper::GetScreenSize().y / sprite->texture.getSize().y);
			sprite->sprite.setColor(Color(sprite->sprite.getColor().r, sprite->sprite.getColor().g, sprite->sprite.getColor().b, 10));
		}
		else
		{
			auto size = Vector2i(sprite->texture.getSize());
			if (size.x > maxSize.x)
				maxSize.x = size.x;

			if (size.y > maxSize.y)
				maxSize.y = size.y;
		}
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

//initialize items
void World::initializeRoseTree(Vector2f position, int typeOfImage, std::string itemName)
{
	int roseTreeType;
	if (typeOfImage == 0)
		roseTreeType = rand() % 3 + 1;
	else
		roseTreeType = typeOfImage;
	std::string nameOfImage = "roseTree" + std::to_string(roseTreeType) + ".png";
	std::string name;
	if (itemName == "")	
		name = nameOfImage + std::to_string(staticGrid.getSize());
	else
		name = itemName;
	staticGrid.addItem(new RoseTree(name, position, roseTreeType), name, int(position.x), int(position.y));

	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	staticGrid.getItemByName(name)->setTextureSize(textureSize);
}
void World::initializeSpawn(Vector2f position, int typeOfImage)
{
	int spawnType;
	if (typeOfImage == 0)
		spawnType = rand() % 4 + 1;
	else
		spawnType = typeOfImage;
	std::string nameOfImage = "spawn" + std::to_string(spawnType) + ".png";
	std::string name = nameOfImage + std::to_string(staticGrid.getSize());
	staticGrid.addItem(new Spawn(name, position), name, int(position.x), int(position.y));

	staticGrid.getItemByName(name)->setTypeOfImage(spawnType);
	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	staticGrid.getItemByName(name)->setTextureSize(textureSize);
}
void World::initializeGrass(Vector2f position, int typeOfImage, std::string itemName)
{	
	int grassType;
	if (typeOfImage == 0)
		grassType = rand() % 5 + 1;
	else
		grassType = typeOfImage;
	std::string nameOfImage = "grass" + std::to_string(grassType) + ".png";
	std::string name;
	if (itemName == "")
		name = nameOfImage + std::to_string(staticGrid.getSize());
	else
		name = itemName;		
	staticGrid.addItem(new Grass(name, position, grassType), name, int(position.x), int(position.y));
	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	staticGrid.getItemByName(name)->setTextureSize(textureSize);
}
void World::initializeEnemy(Vector2f position)
{
	std::string enemyName = "enemy" + std::to_string(dynamicGrid.getSize());
	dynamicGrid.addItem(new Enemy(enemyName, position), enemyName, int(position.x), int(position.y));
	auto enemyTextureBounds = spriteMap[heroTextureName].sprite.getGlobalBounds();
	auto enemyTextureSize = Vector2i(int(enemyTextureBounds.width), int(enemyTextureBounds.height));
	dynamicGrid.getItemByName(enemyName)->setTextureSize(enemyTextureSize);
}
void World::initializeHero(Vector2f position)
{
	dynamicGrid.addItem(new Deerchant(heroName, position), heroName, int(position.x), int(position.y));

	focusedObject = dynamicGrid.getItemByName(heroName);

	auto textureBounds = spriteMap[heroTextureName].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));

	focusedObject->setTextureSize(textureSize);
	cameraPosition = focusedObject->getPosition();
}

void World::Load()
{
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize);
	//pre-initialize
	initializeRoseTree(Vector2f(0, 0), 3, "roseTree");
	//--
	std::ifstream fin("save.txt");
	std::string name;
	float posX, posY;
	int num;
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> name >> posX >> posY;
		std::string curName = Helper::withoutNums(name);
		if (curName == "enemy")
			initializeEnemy(Vector2f(posX, posY));
		else
			if (curName == "hero")
				initializeHero(Vector2f(posX, posY));
	}
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> name >> posX >> posY;
		std::string curName = Helper::getObjectName(name);
		if (Helper::withoutNums(curName) == "roseTree")
			initializeRoseTree(Vector2f(posX, posY), int(curName[8]-48), "");
		else
			if (Helper::withoutNums(curName) == "grass")
				initializeGrass(Vector2f(posX, posY), int(curName[5]-48), "");
	}
	fin.close();
	Save();
}

void World::Save()
{
	std::ofstream fout("save.txt");
	fout.clear();
	staticItems = staticGrid.getItems(0, 0, width, height, width);
	std::vector<DynamicObject*> dynamicItems = dynamicGrid.getItems(0, 0, width, height, width);
	fout << dynamicItems.size() << std::endl;
	for (auto dynamicItem : dynamicItems)
	{
		fout << dynamicItem->getName() << " " << dynamicItem->getPosition().x << " " << dynamicItem->getPosition().y << std::endl;
	}
	fout << staticItems.size() << std::endl;
	for (auto staticItem : staticItems)
	{
		fout << staticItem->getName() << " " << staticItem->getPosition().x << " " << staticItem->getPosition().y << std::endl;
	}
	fout.close();
}

void World::generate(int objCount)
{
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize);
	bossSpawnPosition = Vector2f(width/2, height/2);
	auto s = float(sqrt(objCount));
	
	for (auto i = 0; i < s; i++)
	{
		for (auto j = 0; j < s; j++)
		{			
			auto position = Vector2f(i * (width / s) + rand() % 500, j * (height / s) + rand() % 500);
			initializeRoseTree(position, 0, "");
		}
	}
	//pre-inicialize
	initializeRoseTree(Vector2f(-1000, -1000), 3, "roseTree");
	//--

	for (double i = 0; i < width; i += Grass("test", Vector2f(0,0), 1).conditionalSizeUnits.x / 2)
	{
		for (double j = 0; j < height; j += Grass("test", Vector2f(0, 0), 1).conditionalSizeUnits.y / 2)
		{
			int curType = 0;
			auto position = Vector2f(i, j);
			double distance = sqrt(pow(position.x - bossSpawnPosition.x, 2) + pow(position.y - bossSpawnPosition.y, 2));
			if (distance >= width * 13 / 28)
				curType = 1;
			else
				if (distance >= width * 9 / 28)
					curType = 2;
				else
					if (distance >= width * 5 / 28)
						curType = 3;
					else
						if (distance >= width * 1 / 28)
							curType = 4;
						else
							curType = 5;
							
			initializeGrass(position, curType, "");
		}
	}
	//none target object
	dynamicGrid.addItem(new Enemy("none", Vector2f(width, height)), "none", int(width), int(height));
	//-----------------------------------------
	//test enemy
	initializeEnemy(Vector2f(5000, 5500));
	initializeEnemy(Vector2f(5800, 5600));
	//------------------------------------------
	initializeHero(Vector2f(3000, 3000));
	initializeRoseTree(Vector2f(3500, 3500), 1, "testTree");
	initializeSpawn(Vector2f(3000, 3800), 1);
	Save();
}

void World::ClearWorld()
{
	staticGrid.~GridList();
	dynamicGrid.~GridList();	
}

Vector2f World::newSlippingPositionInCircle(DynamicObject *dynamicItem, Vector2f pos, float radius, long long elapsedTime)
{
	Vector2f motion;
	Vector2f position = dynamicItem->getPosition();

	if (dynamicItem->direction == LEFT || dynamicItem->direction == RIGHT)
	{
		if (dynamicItem->direction == LEFT && radius - (pos.x - position.x) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (dynamicItem->direction == RIGHT && radius - (position.x - pos.x) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y > pos.y)
		{
			motion = Vector2f(0, -dynamicItem->speed * elapsedTime);
			return motion;
		}
		if (position.y < pos.y)
		{
			motion = Vector2f(0, dynamicItem->speed * elapsedTime);
			return motion;
		}	
	}

	if (dynamicItem->direction == UP || dynamicItem->direction == DOWN)
	{
		if (dynamicItem->direction == UP && radius - (pos.y - position.y) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (dynamicItem->direction == DOWN && radius - (position.y - pos.y) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.x > pos.x)
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
		if (position.x < pos.x)
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime, 0);
			return motion;
		}
	}
	if (dynamicItem->direction == UPLEFT)
	{
		if (abs((pos.y - position.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (pos.y - position.y <= radius / sqrt(2))
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime/sqrt(2), -dynamicItem->speed * elapsedTime/sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime / sqrt(2), dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->direction == UPRIGHT)
	{
		if (abs((pos.y - position.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (pos.y - position.y <= radius / sqrt(2))
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime / sqrt(2), -dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime / sqrt(2), dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->direction == DOWNLEFT)
	{
		if (abs((position.y - pos.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y - pos.y <= radius / sqrt(2))
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime / sqrt(2), dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime / sqrt(2), -dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->direction == DOWNRIGHT)
	{
		if (abs((position.y - pos.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y - pos.y <= radius / sqrt(2))
		{
			motion = Vector2f(-dynamicItem->speed * elapsedTime / sqrt(2), dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(dynamicItem->speed * elapsedTime / sqrt(2), -dynamicItem->speed * elapsedTime / sqrt(2));
			return motion;
		}
	}
	return Vector2f(-1000000, -1000000);
}

Vector2f World::newSlippingPosition(DynamicObject *dynamicItem, Vector2f pos, long long elapsedTime)
{
	Vector2f motion;
	Vector2f position = dynamicItem->getPosition();
	float speed = dynamicItem->speed/2;
	//Vector2i pos = Vector2i((terrain->getFocus1().x + terrain->getFocus2().x) / 2, (terrain->getFocus1().y + terrain->getFocus2().y) / 2);
	if (dynamicItem->direction == LEFT || dynamicItem->direction == RIGHT)
	{
		if (position.y <= pos.y)
		{
			motion = Vector2f(0, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(0, speed * elapsedTime);
		return motion;
	}
	if (dynamicItem->direction == UP || dynamicItem->direction == DOWN)
	{
		if (position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, 0);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}
	float a = abs(pos.y - position.y);
	float b = abs(pos.x - position.x);
	float alpha = atan(b / a);
	if (dynamicItem->direction == UPLEFT)
	{		
		if (alpha <= M_PI / 6 || position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == UPRIGHT)
	{
		if (alpha <= M_PI / 6 || position.x >= pos.x)
		{
			motion = Vector2f(speed * elapsedTime, speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->direction == DOWNLEFT)
	{
		if (alpha <= M_PI / 6 || position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}	
	if (dynamicItem->direction == DOWNRIGHT)
	{
		if (alpha <= M_PI / 6 || position.x >= pos.x)
		{
			motion = Vector2f(speed * elapsedTime, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-speed * elapsedTime, 0);
		return motion;
	}
	return Vector2f(-1000000, -1000000);
}

Vector2f World::newSlippingPositionForDynamics(DynamicObject *dynamicItem1, DynamicObject *dynamicItem2, long long elapsedTime)
{
	Vector2f motion;
	Vector2f pos1 = dynamicItem1->getPosition(), pos2 = dynamicItem2->getPosition();
	float up = dynamicItem1->radius + dynamicItem2->radius - (pos2.y - pos1.y);
	float right = dynamicItem1->radius + dynamicItem2->radius - (pos1.x - pos2.x);
	float down = dynamicItem1->radius + dynamicItem2->radius - (pos1.y - pos2.y);
	float left = dynamicItem1->radius + dynamicItem2->radius - (pos2.x - pos1.x);

	if (right <= up && right <= down && right <= left && right >= 0)
	{		
		if (dynamicItem1->direction == LEFT || dynamicItem1->direction == UPLEFT || dynamicItem1->direction == DOWNLEFT)
		{
			motion = Vector2f(0, dynamicItem1->speed * elapsedTime);
			return motion;
		}
	}

	if (down <= right && down <= up && down <= left && down >= 0)
	{
		if (dynamicItem1->direction == UP || dynamicItem1->direction == UPLEFT || dynamicItem1->direction == UPRIGHT || dynamicItem1->direction == RIGHT)
		{
			motion = Vector2f(-dynamicItem1->speed * elapsedTime, 0);	
			return motion;
		}
	}	

	if (left <= right && left <= down && left <= up && left >= 0)
	{
		if (dynamicItem1->direction == RIGHT || dynamicItem1->direction == UPRIGHT || dynamicItem1->direction == DOWNRIGHT)
		{
			motion = Vector2f(0, -dynamicItem1->speed * elapsedTime);
			return motion;
		}
	}

	if (up <= right && up <= down && up <= left && up >= 0)
	{
		if (dynamicItem1->direction == DOWN || dynamicItem1->direction == DOWNLEFT || dynamicItem1->direction == DOWNRIGHT || dynamicItem1->direction == LEFT)
		{
			motion = Vector2f(dynamicItem1->speed * elapsedTime, 0);
			return motion;
		}
	}

	return Vector2f(-1000000, -1000000);
}

VictimSide World::getVictimSide(DynamicObject& hero, DynamicObject& victim)
{
	VictimSide answer;
	Vector2f heroPos = hero.getPosition(), victimPos = victim.getPosition();
	float alpha = atan((victimPos.y - heroPos.y) / (victimPos.x - heroPos.x)) * 180 / pi;
	if (heroPos.y >= victimPos.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = upSide;
	else
		if (heroPos.x <= victimPos.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = rightSide;
		else
			if (heroPos.y <= victimPos.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = downSide;
			else
				if (heroPos.x >= victimPos.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = leftSide;
	return answer;
}

void World::heroHit(DynamicObject& heroObject, DynamicObject& victim, float elapsedTime)
{
	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(heroName));
	if (hero && victim.currentAction != dead)
	{
		if (victim.getName() != heroName && isIntersectDynamic(*hero, hero->getPosition(), victim) && victim.isTransparent/* getVictimSide(heroObject, victim) == hero->hitDirection*/)
		{	
			if (victim.timeForNewHitself >= victim.timeAfterHitself)
			{
				
				if (hero->currentAction == commonHit && hero->getCurrentSprite() == 4)
				{
					hero->addEnergy(5);
					victim.takeDamage(hero->getStrength());
					victim.timeForNewHitself = 0;
				}
				else
				if (hero->currentAction == hardHit && hero->getCurrentSprite() == 6)
				{
					hero->addEnergy(15);
					victim.takeDamage(hero->getStrength()*1.5);
					victim.timeForNewHitself = 0;
				}
				else
				if (hero->currentAction == specialHit && hero->getCurrentSprite() == 5)
				{					
					victim.takeDamage(hero->getStrength()*2);
					victim.timeForNewHitself = 0;
				}
				
			}
				
		}
	}
	if (victim.getHealthPoint() <= 0)
		victim.currentAction = dead;
	victim.timeForNewHitself += elapsedTime;
}

void World::setTransparent(std::vector<WorldObject*> visibleItems)
{
	Vector2i mousePos = Vector2i((Mouse::getPosition().x - Helper::GetScreenSize().x/2 + cameraPosition.x*scaleFactor)/scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	float minDistance = 1000000000;
	for (auto visibleItem : visibleItems)
	{		
		Vector2f itemSize = (Vector2f)visibleItem->getTextureBoxSize();
		visibleItem->isTransparent = false;
		Vector2f itemPos = Vector2f(visibleItem->getPosition().x - visibleItem->getTextureBoxOffset().x, visibleItem->getPosition().y - visibleItem->getTextureBoxOffset().y);
		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getTextureBoxSize().x && mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getTextureBoxSize().y && visibleItem->isTerrain == false)
		{
			visibleItem->isTransparent = true;
			float distanceToBounds = sqrt(pow(mousePos.x - itemPos.x, 2) + pow(mousePos.y - itemPos.y, 2)) +
				sqrt(pow(itemPos.x + itemSize.x - mousePos.x, 2) + pow(mousePos.y - itemPos.y, 2)) +
				sqrt(pow(mousePos.x - itemPos.x, 2) + pow(itemPos.y + itemSize.y - mousePos.y, 2)) +
				sqrt(pow(itemPos.x + itemSize.x - mousePos.x, 2) + pow(itemPos.y + itemSize.y - mousePos.y, 2));
			if (distanceToBounds < minDistance)
			{
				minDistance = distanceToBounds;
				mouseDisplayName = Helper::getObjectName(Helper::withoutNums(visibleItem->getName()));
			}
		}
	}
}

void World::hitInteract(DynamicObject& currentItem, float elapsedTime)
{
	if (currentItem.getName() == "none")
		return;
	if (currentItem.getName() != heroName)
		heroHit(*dynamicGrid.getItemByName(heroName), currentItem, elapsedTime);

	auto enemy = dynamic_cast<Enemy*>(&currentItem);
	if (enemy)
	{
		enemy->hitDirection = (HitDirection)getVictimSide(*enemy, *focusedObject);
		enemy->behavior(*focusedObject);
	}
}

bool World::isClimbBeyond(Vector2f pos)
{
	auto screenSize = Helper::GetScreenSize();
	const auto extra = staticGrid.getBlockSize();

	if (pos.x < screenSize.x / 2 + extra.x)
		return false;
	if (pos.x > width - screenSize.x / 2 - extra.x)
		return false;
	if (pos.y < screenSize.y / 2 + extra.y)
		return false;
	if (pos.y > height - screenSize.y / 2 - extra.y)
		return false;
	return true;
}

void World::interact(RenderWindow& window, long long elapsedTime)
{
	scaleSmoothing();

	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();
	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto characterPosition = focusedObject->getPosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenSize.x / 2), int(characterPosition.y - screenSize.y / 2));
	Vector2i worldBottomRight(int(characterPosition.x + screenSize.x / 2), int(characterPosition.y + screenSize.y / 2));

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y, width);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y, width);

	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(heroName));

	for (auto dynamicItem : dynamicItems)
	{		
		hitInteract(*dynamicItem, elapsedTime);

		if (dynamicItem->direction == STAND)
			continue;

		auto intersects = false;
		auto newPosition = move(*dynamicItem, elapsedTime);
		if (newPosition.x < screenSize.x / 2 + extra.x)
			newPosition.x = screenSize.x / 2 + extra.x;
		if (newPosition.x > width - screenSize.x / 2 - extra.x)
			newPosition.x = width - screenSize.x / 2 - extra.x;
		if (newPosition.y < screenSize.y / 2 + extra.y)
			newPosition.y = screenSize.y / 2 + extra.y;
		if (newPosition.y > height - screenSize.y / 2 - extra.y)
			newPosition.y = height - screenSize.y / 2 - extra.y;

		for (auto staticItem : localStaticItems)
		{
			if (staticItem->isTerrain == true)
				continue;
			auto terrain = dynamic_cast<TerrainObject*>(staticItem);
			if (!terrain)
				continue;			

			if (isIntersectTerrain(*dynamicItem, newPosition, *terrain))
			{
				Vector2f terrainPos = Vector2f((terrain->getFocus1().x + terrain->getFocus2().x) / 2, (terrain->getFocus1().y + terrain->getFocus2().y) / 2);
				auto motionAfterSlipping = newSlippingPosition(dynamicItem, terrainPos, elapsedTime);
				if (motionAfterSlipping.x != -1000000 && motionAfterSlipping.y != -1000000 && isClimbBeyond(Vector2f(newPosition.x + motionAfterSlipping.x, newPosition.y + motionAfterSlipping.y)))
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

		if (dynamicItem->getName() != heroName)
		{
			for (auto otherDynamicItem : dynamicItems)
			{
				if (otherDynamicItem == dynamicItem || otherDynamicItem->getName() == heroName) 
					continue;

				if (isIntersectDynamic(*dynamicItem, newPosition, *otherDynamicItem))
				{
					auto motionAfterSlipping = newSlippingPositionForDynamics(dynamicItem, otherDynamicItem, elapsedTime);
					if (motionAfterSlipping.x != -1000000 && motionAfterSlipping.y != -1000000)
					{
						newPosition = Vector2f(dynamicItem->getPosition().x + motionAfterSlipping.x, dynamicItem->getPosition().y + motionAfterSlipping.y);
						continue;
					}
					intersects = true;
					break;
				}
			}
		}
		else
		{
			if (hero->isFightWithBoss)
			{
				if (sqrt(pow(newPosition.x - bossSpawnPosition.x, 2) + pow(newPosition.y - bossSpawnPosition.y, 2)) >= width * 1 / 28)
				{
					auto motionAfterSlipping = newSlippingPositionInCircle(hero, bossSpawnPosition, width * 1 / 28, elapsedTime);
					if (motionAfterSlipping.x != -1000000 && motionAfterSlipping.y != -1000000)
					{
						newPosition = Vector2f(hero->getPosition().x + motionAfterSlipping.x, hero->getPosition().y + motionAfterSlipping.y);
						hero->setPosition(newPosition);
						dynamicGrid.updateItemPosition(heroName, newPosition.x, newPosition.y);
						continue;
					}					
				}
			}
			else
			if (sqrt(pow(newPosition.x - bossSpawnPosition.x, 2) + pow(newPosition.y - bossSpawnPosition.y, 2)) <= width * 1 / 28)
			{
				hero->isFightWithBoss = true;
			}			
		}

		//if (intersects)
			//continue;

		//auto currentPosition = dynamicItem->getPosition();
		//auto newX = int(newPosition.x);
		//auto newY = int(newPosition.y);		

		//if (newX != int(currentPosition.x) || newY != int(currentPosition.y))
		{
			dynamicItem->setPosition(newPosition);
			dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);
		}
	}	

	timeForNewSave += elapsedTime;
	if (timeForNewSave >= timeAfterSave)
	{
		timeForNewSave = 0;
		Save();		
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	//shaders logic
	spiritWorldShader.setUniform("time", timer.getElapsedTime().asSeconds() / 5);
	spiritWorldShader.setUniform("level", 0);

	const auto extra = staticGrid.getBlockSize();

	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);
	cameraPosition.x += (focusedObject->getPosition().x - cameraPosition.x) * pow(focusedObject->speed, 1.65) * elapsedTime;
	cameraPosition.y += (focusedObject->getPosition().y - cameraPosition.y) * pow(focusedObject->speed, 1.65) * elapsedTime;
	worldUpperLeft = Vector2i(int(cameraPosition.x - (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y - (screenCenter.y + extra.y) / scaleFactor));
	worldBottomRight = Vector2i(int(cameraPosition.x + (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y + (screenCenter.y + extra.y) / scaleFactor));
	if (worldUpperLeft.x < 0)
		worldUpperLeft.x = 0;
	if (worldUpperLeft.y < 0)
		worldUpperLeft.y = 0;
	if (worldBottomRight.x > width)
		worldBottomRight.x = width;
	if (worldBottomRight.y > height)
		worldBottomRight.y = height;

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y, width);
	auto dynamicItems = dynamicGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y, width);
	visibleItems = std::vector<WorldObject*>(localStaticItems.begin(), localStaticItems.end());
	auto visibleDynamicItems = std::vector<WorldObject*>(dynamicItems.begin(), dynamicItems.end());

	visibleItems.insert(visibleItems.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());
	setTransparent(visibleItems);
	sort(visibleItems.begin(), visibleItems.end(), cmpImgDraw);

	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->getPosition();
		auto worldTextureOffset = worldItem->getTextureOffset();
		auto worldTextureSize = worldItem->getTextureSize();

		auto spriteLeft = float((worldItemPosition.x - cameraPosition.x - worldTextureOffset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - cameraPosition.y + (worldItem->getTextureBoxSize().y - worldTextureOffset.y)) * scaleFactor + screenCenter.y);
		auto spriteRight = float(spriteLeft + worldTextureSize.x * scaleFactor);
		auto spriteBottom = float(spriteTop + worldTextureSize.y * scaleFactor);

		auto sprite = (&spriteMap[worldItem->getSpriteName(elapsedTime)])->sprite;

		sprite.setPosition(Vector2f(spriteLeft, spriteTop));
		worldItem->bias.x = worldItem->lastPosition.x - (sprite.getPosition().x - screenCenter.x)/scaleFactor;
		worldItem->bias.y = worldItem->lastPosition.y - (sprite.getPosition().y - screenCenter.y)/scaleFactor;
		//if (scaleDecrease == 0)
			worldItem->lastPosition = Vector2f((sprite.getPosition().x - screenCenter.x)/scaleFactor, (sprite.getPosition().y - screenCenter.y) / scaleFactor);
				

		Vector2f biasOffset = worldItem->getBias(focusedObject->getPosition(), elapsedTime/100);
		sprite.setPosition(Vector2f(sprite.getPosition().x + biasOffset.x, sprite.getPosition().y + biasOffset.y));
		sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);		
		
		if (worldItem->isTransparent)
			//positioning.x = focusedObject->getPosition().x - lastPosition.x;
			positioning.x = scaleDecrease;

		if (!worldItem->isTerrain)				
			sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor*sqrt(sqrt(scaleFactor)));
		else
			sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor);

		//sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor);
		if (worldItem->isTransparent)					
			sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 128));				

		if (focusedObject->getCurrentWorldName() == "spirit" && !worldItem->isTransparent)				
			window.draw(sprite, &spiritWorldShader);		
		else
			window.draw(sprite);
		
		if (worldItem->isTransparent)		
			Helper::drawText(mouseDisplayName, 30, Mouse::getPosition().x + 20, Mouse::getPosition().y + 20, &window);				
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
	rectangle3.setPosition(0 - cameraPosition.x + screenCenter.x, 0 - cameraPosition.y + screenCenter.y);
	window.draw(rectangle3);*/

	/*auto blockSize = staticGrid.getBlockSize();
	for (auto x = 0; x < width; x += blockSize.x)
	for (auto y = 0; y < height; y += blockSize.y)
	{
	auto rectangle4 = RectangleShape();
	rectangle4.setSize(Vector2f(float(blockSize.x), float(blockSize.y)));
	rectangle4.setOutlineColor(Color::Blue);
	rectangle4.setFillColor(Color::Transparent);
	rectangle4.setOutlineThickness(1);
	rectangle4.setPosition(x - characterPosition.x + screenCenter.x, y - characterPosition.y + screenCenter.y);
	window.draw(rectangle4);
	}*/
	lastPosition = focusedObject->getPosition();
}

Vector2f World::move(const DynamicObject& dynamicObject, long long elapsedTime)
{
	auto angle = dynamicObject.direction * M_PI / 180;
	auto position = dynamicObject.getPosition();

	position.x = float(position.x + dynamicObject.speed * cos(angle) * elapsedTime);
	position.y = float(position.y - dynamicObject.speed * sin(angle) * elapsedTime);

	return position;
}

bool World::isIntersectTerrain(DynamicObject& dynamic, Vector2f newPosition, const TerrainObject& terrain) const
{
	auto f1 = terrain.getFocus1();
	auto f2 = terrain.getFocus2();
	//Vector2f position = dynamic.getPosition();
	Vector2f position = newPosition;
	return (sqrt((position.x - f1.x)*(position.x - f1.x) + (position.y - f1.y)*(position.y - f1.y)) + sqrt((position.x - f2.x)*(position.x - f2.x) + (position.y - f2.y)*(position.y - f2.y))/* - dynamic.radius*/) <= terrain.getEllipseSize();
}

bool World::isIntersectDynamic(DynamicObject& dynamic1, Vector2f newPosition, const DynamicObject& dynamic2) const
{
	//Vector2f position1 = dynamic1.getPosition();
	Vector2f position1 = newPosition;
	Vector2f position2 = dynamic2.getPosition();
	return (abs(position1.x - position2.x) <= (dynamic1.radius + dynamic2.radius) && abs(position1.y - position2.y) <= (dynamic1.radius + dynamic2.radius));
	//return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= dynamic1.radius + dynamic2.radius;
}

