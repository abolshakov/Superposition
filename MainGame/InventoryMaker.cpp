#include "InventoryMaker.h"
#include <math.h>
#include <algorithm>

InventoryMaker::InventoryMaker()
{
	heldItemSpeed = 0.00005f;
}

InventoryMaker::~InventoryMaker()
{

}

void InventoryMaker::init()
{
	dropZoneRadius = Helper::GetScreenSize().y / 3;
	heldItem.content = { lootItemsIdList::bagCell, 0 };

	//itemsMaxCount.resize(1000);
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
	while (fin >> spriteName >> offset.x >> offset.y >> id >> maxCount)
	{
		cellsSpriteList.insert({ lootItemsIdList(id), cell() });
		auto itemSprite = &cellsSpriteList[lootItemsIdList(id)].sprite;
		auto itemTexture = &cellsSpriteList[lootItemsIdList(id)].texture;
		itemTexture->loadFromFile("Game/inventorySprites/" + spriteName + ".png");
		itemSprite->setTexture(*itemTexture);
		offset.x *= HeroBag::itemCommonRadius * 2;
		offset.y *= HeroBag::itemCommonRadius * 2;
		cellsSpriteList[lootItemsIdList(id)].offset = offset;
		HeroBag::itemsMaxCount[lootItemsIdList(id)] = maxCount;
	}
	fin.close();

	selectedCellBackground = &cellsSpriteList.at(lootItemsIdList::bagCell).sprite;
	selectedCellBackground->setScale(HeroBag::itemCommonRadius * 2.6 / cellsSpriteList.at(lootItemsIdList::bagCell).texture.getSize().x, HeroBag::itemCommonRadius * 2.6 / cellsSpriteList.at(lootItemsIdList::bagCell).texture.getSize().y);
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

	if (heldItem.content.first != lootItemsIdList::bagCell)
	{
		Vector2f shiftVector = Vector2f((Mouse::getPosition().x - heldItem.position.x)*heldItemSpeed*elapsedTime, (Mouse::getPosition().y - heldItem.position.y)*heldItemSpeed*elapsedTime);
		heldItem.position.x += shiftVector.x; heldItem.position.y += shiftVector.y;
	}
}

void InventoryMaker::onMouseDownInteract()
{
	usedMouse = false;
	const Vector2f mousePos = Vector2f(Mouse::getPosition());

	//cell positioning
	/*for (int i = 0; i < temporaryPositions.size(); i++)
	{
		if (Helper::getDist(mousePos, temporaryPositions[i].first) <= HeroBag::itemCommonRadius)
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
		// bag state changing		
		if (bag.get().readyToEject)
			bag.get().currentState = ejected;
		if (bag.get().currentState != bagOpen)
		{
			if (bag.get().currentState == bagClosed && bag.get().readyToChangeState && !bag.get().wasMoved)
			{
				bag.get().currentState = bagOpening;
				bag.get().wasMoved = false;
				break;
			}
		}
		else
		{
			if (bag.get().currentState == bagOpen && bag.get().readyToChangeState)
			{
				bag.get().currentState = bagClosing;
				bag.get().wasMoved = false;
				break;
			}
		}
		bag.get().wasMoved = false;
		if (bag.get().currentState != bagOpen)
			break;
		//-------------------

		// put cursor item to bag
		if (heldItem.content.first != lootItemsIdList::bagCell)
		{
			int curIndex = bag.get().getSelectedCell(mousePos);
			if (curIndex == -1)
				continue;
			usedMouse = true;
			auto& item = bag.get().cells[curIndex];
			if (item.content.first == lootItemsIdList::bagCell || item.content.first == heldItem.content.first)
			{
				item.content.first = heldItem.content.first;
				item.content.second += heldItem.content.second;
				if (item.content.second > HeroBag::itemsMaxCount.at(lootItemsIdList(item.content.first)))
				{
					heldItem.content.second = item.content.second % HeroBag::itemsMaxCount.at(lootItemsIdList(item.content.first));
					item.content.second = HeroBag::itemsMaxCount.at(lootItemsIdList(item.content.first));
				}
				else
					heldItem.content = { lootItemsIdList::bagCell, 0 };
				break;
			}
			else
			{
				const std::pair<lootItemsIdList, int> temp = heldItem.content;
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
				bag.get().cells[curIndex].content = { lootItemsIdList::bagCell, 0 };
			}
		}
		//-----------------------
	}
}

void InventoryMaker::temporaryInventoryBounding(std::vector<std::reference_wrapper<std::pair <lootItemsIdList, int>>> inventory)
{
	currentInventory = inventory;
	currentInventorySize = inventory.size();
}

void InventoryMaker::drawNumberOfItems(Sprite sprite, int itemsCount, RenderWindow &window)
{
	numberOfItems.setString(std::to_string(itemsCount));
	numberOfItems.setOrigin(numberOfItems.getGlobalBounds().width, numberOfItems.getGlobalBounds().height);
	numberOfItems.setPosition(sprite.getPosition().x + HeroBag::itemCommonRadius * 2, sprite.getPosition().y + HeroBag::itemCommonRadius * 2);
	window.draw(numberOfItems);
}

void InventoryMaker::drawHeroInventory(float elapsedTime, RenderWindow& window)
{
	// draw bags
	for (auto& bag : boundBags)
	{
		bag.get().draw(&window, elapsedTime);
		bag.get().readyToEject = false;
		//bag.get().drawCircuit(&window);
	}
	cursorText = "";
	//drawing bags content
	for (auto bag : boundBags)
	{
		if (Helper::getDist(bag.get().getPosition(), Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2)) <= dropZoneRadius && bag.get().currentState == bagClosed)
		{
			cursorText = "drop on the ground";
			cursorTextPos = bag.get().getPosition();
			bag.get().readyToEject = true;
		}			

		// drawing bag content
		if (bag.get().currentState != bagOpen)
			continue;
		for (int cnt = 0; cnt < bag.get().cells.size(); cnt++)
		{
			auto& item = bag.get().cells[cnt];
			//drawing cell background
			if (bag.get().getSelectedCell(Vector2f(Mouse::getPosition())) == cnt)
			{
				Vector2f backgroundOffset = cellsSpriteList.at(lootItemsIdList::bagCell).offset;
				selectedCellBackground->setPosition(item.position.x - HeroBag::itemCommonRadius - backgroundOffset.x, item.position.y - HeroBag::itemCommonRadius - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
			//-----------------------

			if (bag.get().cells[cnt].content.first == lootItemsIdList::bagCell)
				continue;

			auto sprite = cellsSpriteList.at(lootItemsIdList(item.content.first)).sprite;
			sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
			Vector2f offset = cellsSpriteList.at(lootItemsIdList(item.content.first)).offset;
			sprite.setPosition(Vector2f(item.position.x - HeroBag::itemCommonRadius - offset.x, item.position.y - HeroBag::itemCommonRadius - offset.y));
			sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
			window.draw(sprite);

			if (HeroBag::itemsMaxCount.at(item.content.first) != 1)
				drawNumberOfItems(sprite, item.content.second, window);
		}
	}
	//--------------------

	//drawing held item
	if (heldItem.content.first != lootItemsIdList::bagCell)
	{
		auto sprite = cellsSpriteList.at(lootItemsIdList(heldItem.content.first)).sprite;
		sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
		sprite.setPosition(Vector2f(heldItem.position.x - HeroBag::itemCommonRadius, heldItem.position.y - HeroBag::itemCommonRadius));
		window.draw(sprite);
		if (HeroBag::itemsMaxCount.at(heldItem.content.first) != 1)
			drawNumberOfItems(sprite, heldItem.content.second, window);
	}
	//-----------------

	//cell positioning visualization
	/*
	for (auto pos : temporaryPositions)
	{
		for (int i = pos.first.x - HeroBag::itemCommonRadius; i <= pos.first.x + HeroBag::itemCommonRadius; i+= 10)
			for (int j = pos.first.y - HeroBag::itemCommonRadius; j <= pos.first.y + HeroBag::itemCommonRadius; j += 10)
			{
				if (Helper::getDist(Vector2f(i, j), pos.first) <= HeroBag::itemCommonRadius)
				{
					RectangleShape rec;
					rec.setPosition(i, j);
					rec.setSize(Vector2f(5, 5));
					rec.setFillColor(Color::Red);
					window.draw(rec);
					Helper::drawText(pos.second, 20, pos.first.x - HeroBag::itemCommonRadius, pos.first.y - HeroBag::itemCommonRadius / 2, &window);
				}
			}
	}*/

	// draw cursor text
	if (cursorText == "drop on the ground")
		textWriter.drawTextBox(cursorText, NormalFont, 35 * Helper::GetScreenSize().y / 1440, cursorTextPos.x - Helper::GetScreenSize().x / 24, cursorTextPos.y - Helper::GetScreenSize().y / 24, 
			Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 12, &window, Color::White);
}

void InventoryMaker::drawInventory(Vector2f position, float elapsedTime, RenderWindow& window)
{

}

void InventoryMaker::resetAnimationValues()
{
	animationCounter = 0;
	timeAfterAnimationEffect = 0;
}
