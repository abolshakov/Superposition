#include "InventoryMaker.h"
#include <math.h>
#include <algorithm>

InventoryMaker::InventoryMaker()
{

}

InventoryMaker::~InventoryMaker()
{

}

void InventoryMaker::init()
{
	initSpriteList();
	font.loadFromFile("fonts/Bebas.ttf");
	numberOfObjects.setFont(font);
	numberOfObjects.setCharacterSize(30);
	numberOfObjects.setFillColor(Color(200, 200, 200, 255));
}

void InventoryMaker::initSpriteList()
{
	Vector2f screenSize = Helper::GetScreenSize();
	std::string spriteName;
	int id, maxCount;
	std::ifstream fin(spritesFileDirectory);
	while (fin >> spriteName >> id >> maxCount)
	{
		cellsSpriteList.insert({ id, cell() });
		auto itemSprite = &cellsSpriteList[id].sprite;
		auto itemTexture = &cellsSpriteList[id].texture;
		itemTexture->loadFromFile("Game/inventorySprites/" + spriteName + ".png");
		itemSprite->setTexture(*itemTexture);
		itemsMaxCount.push_back(maxCount);
	}
	fin.close();

	cellSize = Vector2f (screenSize.x / 30, screenSize.x / 30);

	heroInventoryBackgroundTexture.loadFromFile("Game/inventorySprites/box.png");
	heroInventoryBackgroundSprite.setTexture(heroInventoryBackgroundTexture);
	heroInventoryBackgroundSprite.setOrigin(0, heroInventoryBackgroundTexture.getSize().y);
	heroInventoryBackgroundSprite.setPosition(Vector2f(Helper::GetScreenSize().x / 1.5, Helper::GetScreenSize().y));
	heroInventoryBackgroundSprite.setScale((Helper::GetScreenSize().y / 1.75) / heroInventoryBackgroundTexture.getSize().x, (Helper::GetScreenSize().y / 6) / heroInventoryBackgroundTexture.getSize().y);
	heroInventoryZoneSize = Vector2f(Helper::GetScreenSize().y / 1.75, Helper::GetScreenSize().y / 6);
}

void InventoryMaker::inventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory)
{
	boundInventory = inventory;
	heroInventoryCellSize = Vector2f (heroInventoryZoneSize.x / 8, heroInventoryZoneSize.x / 8);
}

void InventoryMaker::onMouseDownInteract()
{
	if (currentHeroInventoryCell != -1)
	{
		if (heldItem == std::make_pair(-1, -1))
		{
			if (boundInventory[currentHeroInventoryCell].get().first != 0)
			{
				heldItem = boundInventory[currentHeroInventoryCell].get();
				boundInventory[currentHeroInventoryCell].get() = { 0, 0 };
			}
		}
		else
		{
			if (boundInventory[currentHeroInventoryCell].get().first == 0)
			{
				boundInventory[currentHeroInventoryCell].get() = heldItem;
				heldItem = std::make_pair(-1, -1);
			}
			else
				if (boundInventory[currentHeroInventoryCell].get().first == heldItem.first)
				{
					if (boundInventory[currentHeroInventoryCell].get().second + heldItem.second <= itemsMaxCount[heldItem.first])
					{
						boundInventory[currentHeroInventoryCell].get().second += heldItem.second;
						heldItem = std::make_pair(-1, -1);
					}
					else
					{
						heldItem.second -= itemsMaxCount[heldItem.first] - boundInventory[currentHeroInventoryCell].get().second;
						boundInventory[currentHeroInventoryCell].get().second = itemsMaxCount[heldItem.first];						
					}
				}
				else
					if (boundInventory[currentHeroInventoryCell].get().first != heldItem.first)
					{
						swap(boundInventory[currentHeroInventoryCell].get(), heldItem);
					}
		}
	}

	if (currentCell != -1 && currentInventory.size() != 0)
		putItemToBound();
}

void InventoryMaker::temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory)
{
	currentInventory = inventory;
	currentInventorySize = inventory.size();
}

void InventoryMaker::putItemToBound()
{
	for (auto curInvItem = boundInventory.begin(); curInvItem != boundInventory.end(); ++curInvItem)
	{
		if (curInvItem->get().first == currentInventory[currentCell].get().first || curInvItem->get().first == 0)
		{
			if (curInvItem->get().second + currentInventory[currentCell].get().second <= itemsMaxCount[currentInventory[currentCell].get().first])
			{
				curInvItem->get().first = currentInventory[currentCell].get().first;
				curInvItem->get().second += currentInventory[currentCell].get().second;
				//currentInventory.erase(currentInventory.begin() + currentCell);
				currentInventory[currentCell].get().first = 0;
				currentInventory[currentCell].get().second = 0;
				currentInventorySize--;
				return;
			}
			else
			{
				curInvItem->get().first = currentInventory[currentCell].get().first;
				currentInventory[currentCell].get().second -= itemsMaxCount[currentInventory[currentCell].get().first] - curInvItem->get().second;
				curInvItem->get().second = itemsMaxCount[currentInventory[currentCell].get().first];
			}
		}
	}
}

bool cmpInventorySort(std::pair<int, int> a, std::pair<int, int> b)
{
	if (a.first == b.first)
		return a.second > b.second;
	return a.first > b.first;
}

int InventoryMaker::getSelectedCellNumber()
{
	Vector2f screenSize = Helper::GetScreenSize();
	int answer = 0;
	Vector2f mousePosition = Vector2f (Mouse::getPosition());
	if (!Helper::isIntersects(mousePosition, IntRect(currentPosition.x, currentPosition.y, int(ceil(sqrt(currentInventorySize)))*cellSize.x, int(ceil(sqrt(currentInventorySize)))*cellSize.y)))
		return -1;
	Vector2f positionOffset = Vector2f (int(mousePosition.x - currentPosition.x), int(mousePosition.y - currentPosition.y));
	answer += (int(positionOffset.y) / int(cellSize.y)) * int(ceil(sqrt(currentInventorySize)));
	answer += (int(positionOffset.x) / int(cellSize.x));
	if (answer >= visibleItemsNumber)
		return -1;
	else
		return answer;
}

int InventoryMaker::getHeroInventorySelectedCellNumber()
{
	Vector2f screenSize = Helper::GetScreenSize();
	int answer = 0;
	Vector2f mousePosition = Vector2f (Mouse::getPosition());
	if (!Helper::isIntersects(mousePosition, IntRect(currentHeroInventoryPosition.x, currentHeroInventoryPosition.y, 8 * heroInventoryCellSize.x, 2 * heroInventoryCellSize.y)))
		return -1;
	Vector2f positionOffset = Vector2f (int(mousePosition.x - currentHeroInventoryPosition.x), int(mousePosition.y - currentHeroInventoryPosition.y));
	answer += (int(positionOffset.y) / int(heroInventoryCellSize.y)) * 8;
	answer += (int(positionOffset.x) / int(heroInventoryCellSize.x));
	if (answer >= boundInventory.size())
		return -1;
	else
		return answer;
}

void InventoryMaker::drawHeroInventory(float elapsedTime, RenderWindow& window)
{
	if (currentCell != -1 || currentHeroInventoryCell != -1)
		usedMouse = true;
	else
		usedMouse = false;

	if (heldItem.second <= 0)
		heldItem = std::make_pair(-1, -1);

	window.draw(heroInventoryBackgroundSprite);

	currentHeroInventoryPosition = Vector2f (heroInventoryBackgroundSprite.getPosition().x, Helper::GetScreenSize().y - heroInventoryZoneSize.y);

	currentHeroInventoryCell = getHeroInventorySelectedCellNumber();

	Vector2f drawCellPosition = Vector2f (heroInventoryBackgroundSprite.getPosition().x, Helper::GetScreenSize().y - heroInventoryZoneSize.y);

	int inRaw = 0;

	for (int i = 0; i < boundInventory.size(); i++)
	{
		//get sprite from list
		auto sprite = &cellsSpriteList[boundInventory[i].get().first].sprite;
		sprite->setPosition(drawCellPosition.x, drawCellPosition.y);
		sprite->setScale(heroInventoryCellSize.x / sprite->getTextureRect().width, heroInventoryCellSize.y / sprite->getTextureRect().height);

		//new raw
		if ((inRaw+1) * heroInventoryCellSize.x >= heroInventoryZoneSize.x)
		{
			if (inRaw > heroInventoryRawCellsNumber)
				heroInventoryRawCellsNumber = inRaw;
			inRaw = 0;
			drawCellPosition.y += heroInventoryCellSize.y;
			drawCellPosition.x = heroInventoryBackgroundSprite.getPosition().x;
		}
		else
		{
			inRaw++;
			drawCellPosition.x += heroInventoryCellSize.x;
		}
		
		//set transparent
		if (i == currentHeroInventoryCell && boundInventory[i].get().first != 0)
		{
			Color spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 125));
			window.draw(*sprite);
			sprite->setColor(spriteColor);
		}
		else
			window.draw(*sprite);
		
		if (boundInventory[i].get().first != 0 && boundInventory[i].get().second != 0)
		{
			numberOfObjects.setString(std::to_string(boundInventory[i].get().second));
			numberOfObjects.setPosition(sprite->getPosition().x + heroInventoryCellSize.x - numberOfObjects.getGlobalBounds().width, sprite->getPosition().y + heroInventoryCellSize.y - numberOfObjects.getGlobalBounds().height);
			window.draw(numberOfObjects);
		}
	}

	if (heldItem != std::make_pair(-1, -1))
	{
		auto sprite = &cellsSpriteList[heldItem.first].sprite;
		sprite->setPosition(Vector2f(sf::Mouse::getPosition()));
		sprite->setScale(heroInventoryCellSize.x / sprite->getTextureRect().width, heroInventoryCellSize.y / sprite->getTextureRect().height);
		window.draw(*sprite);
	}
}

void InventoryMaker::drawInventory(Vector2f position, float elapsedTime, RenderWindow& window)
{
	Vector2f screenSize = Helper::GetScreenSize();

	if (currentInventory.size() == 0)
		return;

	std::sort(currentInventory.begin(), currentInventory.end(), cmpInventorySort);
	currentCell = getSelectedCellNumber();
	currentPosition = position;

	timeAfterAnimationEffect += elapsedTime;
	
	if (timeAfterAnimationEffect >= timeForAnimationEffect)
	{
		timeAfterAnimationEffect = 0;
		if (animationCounter < currentInventory.size())
			animationCounter++;
		else
			animationCounter = currentInventory.size();
	}

	//animationCounter = currentInventory.size();

	visibleItemsNumber = 0;
	Vector2f drawCellPosition = position;

	for (int i = 0; i < animationCounter; i++)
	{
		//is empty
		if (currentInventory[i].get().first == 0)
		{
			continue;
		}

		//get sprite from list
		visibleItemsNumber++;
		auto sprite = &cellsSpriteList[currentInventory[i].get().first].sprite;
		sprite->setPosition(drawCellPosition.x, drawCellPosition.y);
		sprite->setScale(cellSize.x / sprite->getTextureRect().width, cellSize.y / sprite->getTextureRect().height);

		//new raw
		if ((i + 1) % int(ceil(sqrt(currentInventory.size()))) == 0)
		{
			drawCellPosition.y += cellSize.y;
			drawCellPosition.x = position.x;
		}
		else
			drawCellPosition.x += cellSize.x;
		//set transparent
		if (i == currentCell)
		{
			Color spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 125));
			window.draw(*sprite);
			sprite->setColor(spriteColor);

			numberOfObjects.setString(std::to_string(currentInventory[i].get().second));
			numberOfObjects.setPosition(sprite->getPosition().x + cellSize.x - numberOfObjects.getGlobalBounds().width, sprite->getPosition().y + cellSize.y - numberOfObjects.getGlobalBounds().height);
			window.draw(numberOfObjects);
		}
		else
			window.draw(*sprite);
	}
}

void InventoryMaker::resetAnimationValues()
{
	animationCounter = 0;
	timeAfterAnimationEffect = 0;
	currentCell = -1;
}
