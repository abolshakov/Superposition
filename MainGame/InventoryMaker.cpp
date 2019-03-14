#include "InventoryMaker.h"
#include <math.h>
#include <algorithm>

InventoryMaker::InventoryMaker()
{
	itemCommonRadius = Helper::GetScreenSize().y / 36;
	heldItemSpeed = 0.00005f;
}

InventoryMaker::~InventoryMaker()
{

}

void InventoryMaker::init()
{
	heldItem.content = { -1, 0 };

	itemsMaxCount.resize(1000);
	initSpriteList();
	font.loadFromFile("fonts/Bebas.ttf");
	numberOfItems.setFont(font);
	numberOfItems.setCharacterSize(30);
	//numberOfItems.setFillColor(Color(200, 200, 200, 255));
	numberOfItems.setFillColor(Color::White);
}

void InventoryMaker::initSpriteList()
{
	Vector2f screenSize = Helper::GetScreenSize();
	std::string spriteName;
	int id, maxCount;
	Vector2f offset;
	std::ifstream fin(spritesFileDirectory);
	while (fin >> spriteName >> offset.x >> offset.y >>  id >> maxCount)
	{
		cellsSpriteList.insert({ id, cell() });
		auto itemSprite = &cellsSpriteList[id].sprite;
		auto itemTexture = &cellsSpriteList[id].texture;
		itemTexture->loadFromFile("Game/inventorySprites/" + spriteName + ".png");
		itemSprite->setTexture(*itemTexture);
		offset.x *= itemCommonRadius * 2;
		offset.y *= itemCommonRadius * 2;
		cellsSpriteList[id].offset = offset;
		itemsMaxCount[id] = maxCount;
	}
	fin.close();

	selectedCellBackground = &cellsSpriteList.at(0).sprite;
	selectedCellBackground->setScale(itemCommonRadius * 2.6 / cellsSpriteList.at(0).texture.getSize().x, itemCommonRadius * 2.6 / cellsSpriteList.at(0).texture.getSize().y);
	selectedCellBackground->setColor(Color(selectedCellBackground->getColor().r, selectedCellBackground->getColor().g, selectedCellBackground->getColor().b, 125));
}

void InventoryMaker::inventoryBounding(std::vector<std::reference_wrapper<HeroBag>> bags)
{
	boundBags = bags;
}

void InventoryMaker::interact(float elapsedTime)
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());

	for (auto& bag : boundBags)
	{
		if (bag.get().currentState == bagClosed)
		{
			const Vector2f selectionZonePos = Vector2f(bag.get().getPosition().x + bag.get().selectionZoneClosedOffset.x,
				bag.get().getPosition().y + bag.get().selectionZoneClosedOffset.y);
			if (Helper::getDist(mousePos, selectionZonePos) <= bag.get().selectionZoneRadiusClosed)
				bag.get().readyToChangeState = true;				
			else
				bag.get().readyToChangeState = false;
		}
		else
		if (bag.get().currentState == bagOpen)
		{
			const Vector2f selectionZonePos = Vector2f(bag.get().getPosition().x + bag.get().selectionZoneOpenOffset.x,
				bag.get().getPosition().y + bag.get().selectionZoneOpenOffset.y);
			if (Helper::getDist(mousePos, selectionZonePos) <= bag.get().selectionZoneRadiusOpen)
				bag.get().readyToChangeState = true;
			else
				bag.get().readyToChangeState = false;
		}
	}

	if (heldItem.content.first != -1)
	{
		Vector2f shiftVector = Vector2f((Mouse::getPosition().x - heldItem.position.x)*heldItemSpeed*elapsedTime, (Mouse::getPosition().y - heldItem.position.y)*heldItemSpeed*elapsedTime);
		heldItem.position.x += shiftVector.x; heldItem.position.y += shiftVector.y;
	}
}

void InventoryMaker::onMouseDownInteract()
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());

	//cell positioning
	/*for (int i = 0; i < temporaryPositions.size(); i++)
	{
		if (Helper::getDist(mousePos, temporaryPositions[i].first) <= itemCommonRadius)
		{
			temporaryPositions.erase(temporaryPositions.begin() + i);
			return;
		}
	}
	std::string str1 = std::to_string((Mouse::getPosition().x - boundBags[0].get().getPosition().x) / boundBags[0].get().getSizeOpen().x), 
	str2 = std::to_string((Mouse::getPosition().y - boundBags[0].get().getPosition().y) / boundBags[0].get().getSizeOpen().y);
	std::string toSave = str1.substr(0, 5) + " " + str2.substr(0, 5);

	temporaryPositions.push_back({mousePos, toSave});*/
	//-----------------

	for (auto& bag : boundBags)
	{
		if (bag.get().currentState != bagOpen)
		{
			if (bag.get().currentState == bagClosed && bag.get().readyToChangeState)
			{
				bag.get().currentState = bagOpening;
				break;
			}
		}
		else
		{
			if (bag.get().currentState == bagOpen && bag.get().readyToChangeState)
			{
				bag.get().currentState = bagClosing;
				break;
			}
		}

		if (heldItem.content.first != -1)
		{
			int curIndex = bag.get().getSelectedCell(mousePos);
			if (curIndex == -1)
				continue;

			auto& item = bag.get().cells[curIndex];
			if (item.content.first == -1 || item.content.first == heldItem.content.first)
			{
				item.content.first = heldItem.content.first;
				item.content.second += heldItem.content.second;
				if (item.content.second > itemsMaxCount[item.content.first])
				{
					heldItem.content.second = item.content.second % itemsMaxCount[item.content.first];
					item.content.second = itemsMaxCount[item.content.first];
				}
				else
					heldItem.content = { -1, 0 };
				break;
			}
			else
			{
				const std::pair<int, int> temp = heldItem.content;
				heldItem.content = item.content;
				item.content = temp;
			}
		}
		else
		{
			int curIndex = bag.get().getSelectedCell(mousePos);
			if (curIndex != -1)
			{
				heldItem.content = bag.get().cells[curIndex].content;
				heldItem.position = bag.get().cells[curIndex].position;
				bag.get().cells[curIndex].content = { -1, 0 };
			}
		}
	}
}

void InventoryMaker::temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <int, int>>> inventory)
{
	currentInventory = inventory;
	currentInventorySize = inventory.size();
}

bool cmpInventorySort(std::pair<int, int> a, std::pair<int, int> b)
{
	if (a.first == b.first)
		return a.second > b.second;
	return a.first > b.first;
}

void InventoryMaker::drawNumberOfItems(Sprite sprite, int itemsCount, RenderWindow &window)
{
	numberOfItems.setString(std::to_string(itemsCount));
	numberOfItems.setOrigin(numberOfItems.getGlobalBounds().width, numberOfItems.getGlobalBounds().height);
	numberOfItems.setPosition(sprite.getPosition().x + itemCommonRadius * 2, sprite.getPosition().y + itemCommonRadius * 2);
	window.draw(numberOfItems);
}

void InventoryMaker::drawHeroInventory(float elapsedTime, RenderWindow& window)
{
	for (auto& bag : boundBags)
	{
		bag.get().draw(&window, elapsedTime);
		//bag.get().drawCircuit(&window);
	}

	//drawing bags content
	for (auto bag : boundBags)
	{
		if (bag.get().currentState != bagOpen)
			continue;

		for (int cnt = 0; cnt < bag.get().cells.size(); cnt++)
		{
			auto& item = bag.get().cells[cnt];

			//drawing cell background
			if (bag.get().getSelectedCell(Vector2f(Mouse::getPosition())) == cnt)
			{
				Vector2f backgroundOffset = cellsSpriteList.at(0).offset;
				selectedCellBackground->setPosition(item.position.x - itemCommonRadius - backgroundOffset.x, item.position.y - itemCommonRadius - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
			//-----------------------

			if (bag.get().cells[cnt].content.first == -1)
				continue;

			auto sprite = cellsSpriteList.at(item.content.first).sprite;
			sprite.setScale(itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
			Vector2f offset = cellsSpriteList.at(item.content.first).offset;
			sprite.setPosition(Vector2f(item.position.x - itemCommonRadius - offset.x, item.position.y - itemCommonRadius - offset.y));
			sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
			window.draw(sprite);

			drawNumberOfItems(sprite, item.content.second, window);
		}
	}
	//--------------------

	//drawing held item
	if (heldItem.content.first != -1)
	{
		auto sprite = cellsSpriteList.at(heldItem.content.first).sprite;
		sprite.setScale(itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
		sprite.setPosition(Vector2f(heldItem.position.x - itemCommonRadius, heldItem.position.y - itemCommonRadius));
		window.draw(sprite);
		drawNumberOfItems(sprite, heldItem.content.second, window);
	}
	//-----------------

	//cell positioning visualization
	/*
	for (auto pos : temporaryPositions)
	{
		for (int i = pos.first.x - itemCommonRadius; i <= pos.first.x + itemCommonRadius; i+= 10)
			for (int j = pos.first.y - itemCommonRadius; j <= pos.first.y + itemCommonRadius; j += 10)
			{
				if (Helper::getDist(Vector2f(i, j), pos.first) <= itemCommonRadius)
				{
					RectangleShape rec;
					rec.setPosition(i, j);
					rec.setSize(Vector2f(5, 5));
					rec.setFillColor(Color::Red);
					window.draw(rec);
					Helper::drawText(pos.second, 20, pos.first.x - itemCommonRadius, pos.first.y - itemCommonRadius / 2, &window);
				}
			}
	}*/
	//Helper::drawText(debugInfo, 30, 2000, 500, &window);
}

void InventoryMaker::drawInventory(Vector2f position, float elapsedTime, RenderWindow& window)
{

}

void InventoryMaker::resetAnimationValues()
{
	animationCounter = 0;
	timeAfterAnimationEffect = 0;
}
