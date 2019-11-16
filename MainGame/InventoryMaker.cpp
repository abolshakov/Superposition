#include "InventoryMaker.h"
#include "Helper.h"
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
	dropZoneRadius = Helper::GetScreenSize().y * 2 / 7;
	heldItem.content = { Tag::emptyCell, 0 };
	dropZoneTexture.loadFromFile("Game/inventorySprites/dropZone.png");
	dropZone.setTexture(dropZoneTexture); dropZone.setScale(Helper::GetScreenSize().x / dropZoneTexture.getSize().x, Helper::GetScreenSize().y / dropZoneTexture.getSize().y);
	bagPosDot.setRadius(Helper::GetScreenSize().y / 288);
	bagPosDot.setFillColor(Color(53, 53, 53, 200));

	//itemsMaxCount.resize(1000);
	initSpriteLists();		
}

void InventoryMaker::initBags()
{
	std::string textureOBag, textureOBagS, textureCBag, textureCBagS;
	int id;
	std::ifstream fin(bagsFileDirectory);
	while (fin >> id)
	{
		fin >> textureCBag >> textureCBagS >> textureOBag >> textureOBagS;
		bagsSpriteList.insert({ Tag(id), BagSprite() });
		bagsSpriteList.at(Tag(id)).TextureCBag.loadFromFile(textureCBag);
		bagsSpriteList.at(Tag(id)).TextureCBagS.loadFromFile(textureCBagS);
		bagsSpriteList.at(Tag(id)).TextureOBag.loadFromFile(textureOBag);
		bagsSpriteList.at(Tag(id)).TextureOBagS.loadFromFile(textureOBagS);
		bagsSpriteList.at(Tag(id)).CBag.setTexture(bagsSpriteList.at(Tag(id)).TextureCBag);
		bagsSpriteList.at(Tag(id)).CBagS.setTexture(bagsSpriteList.at(Tag(id)).TextureCBagS);
		bagsSpriteList.at(Tag(id)).OBag.setTexture(bagsSpriteList.at(Tag(id)).TextureOBag);
		bagsSpriteList.at(Tag(id)).OBagS.setTexture(bagsSpriteList.at(Tag(id)).TextureOBagS);
	}
	fin.close();
}

void InventoryMaker::initCells()
{
	std::string spriteName;
	int id, maxCount;
	Vector2f offset;
	std::ifstream fin(cellsFileDirectory);
	while (fin >> spriteName >> offset.x >> offset.y >> id >> maxCount)
	{
		cellsSpriteList.insert({ Tag(id), CellSprite() });
		auto itemSprite = &cellsSpriteList[Tag(id)].sprite;
		auto itemTexture = &cellsSpriteList[Tag(id)].texture;
		itemTexture->loadFromFile("Game/inventorySprites/" + spriteName + ".png");
		itemSprite->setTexture(*itemTexture);
		offset.x *= HeroBag::itemCommonRadius * 2;
		offset.y *= HeroBag::itemCommonRadius * 2;
		cellsSpriteList[Tag(id)].offset = offset;
		HeroBag::itemsMaxCount[Tag(id)] = maxCount;
	}
	fin.close();
}

void InventoryMaker::initSpriteLists()
{
	initBags();
	initCells();

	selectedCellBackground = &cellsSpriteList.at(Tag::emptyCell).sprite;
	selectedCellBackground->setScale(HeroBag::itemCommonRadius * 2.6 / cellsSpriteList.at(Tag::emptyCell).texture.getSize().x, HeroBag::itemCommonRadius * 2.6 / cellsSpriteList.at(Tag::emptyCell).texture.getSize().y);
	selectedCellBackground->setColor(Color(selectedCellBackground->getColor().r, selectedCellBackground->getColor().g, selectedCellBackground->getColor().b, 125));
}

void InventoryMaker::inventoryBounding(std::vector<HeroBag>* bags)
{
	boundBags = bags;
}

void InventoryMaker::moveOtherBags(int cur, std::vector<int> ancestors)
{
	const int lapsCount = 20;
	//if (boundBags->at(cur).currentState != bagClosed)
	//{
		for (auto& anotherBag : *boundBags)
		{
			if (anotherBag.getPosition() == boundBags->at(cur).getPosition() || anotherBag.wasMoved)
				continue;
			const Vector2f newPos = Vector2f(boundBags->at(cur).getPosition().x + boundBags->at(cur).shiftVector.x, boundBags->at(cur).getPosition().y + boundBags->at(cur).shiftVector.y);

			if (Helper::getDist(newPos, anotherBag.getPosition()) < boundBags->at(cur).getRadius() + anotherBag.getRadius())
			{

				float k = 0.05 * lapsCount * ((boundBags->at(cur).getRadius() + anotherBag.getRadius()) - Helper::getDist(newPos, anotherBag.getPosition())) / Helper::getDist(newPos, anotherBag.getPosition());
				anotherBag.shiftVector = Vector2f((anotherBag.getPosition().x - newPos.x) * k, (anotherBag.getPosition().y - newPos.y) * k);

				Vector2f tempNewPos = Vector2f(anotherBag.getPosition().x + anotherBag.shiftVector.x, anotherBag.getPosition().y + (anotherBag.shiftVector.y));
				anotherBag.movePosition = tempNewPos;
				anotherBag.shiftVector.x = 0; anotherBag.shiftVector.y = 0;

				//break;
			}
		}
	//}
}

void InventoryMaker::interact(float elapsedTime)
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());

	int cnt = -1; minDistToClosed = 10e4; minDistToOpen = 10e4;
	int minDistToClosedIndex = 0, minDistToOpenIndex = 0;
	for (auto& bag : *boundBags)
	{
		cnt++;

		if (!bag.wasBounded && bagsSpriteList.count(Tag::heroBag) > 0)
			bag.boundSprites(&(bagsSpriteList.at(Tag::heroBag)));

		if (cursorText == "  throw away" && Helper::getDist(bag.getPosition(), Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2)) <= dropZoneRadius ||
			cursorText == "")
			if (cnt == currentMovingBag || currentMovingBag == -1)
				bag.mouseMove();

		//move other bags while opening
		moveOtherBags(cnt);
		//-----------------------------

		// bag auto-moving		
		if (bag.movePosition != Vector2f(0, 0) && Helper::getDist(bag.getPosition(), bag.movePosition) > bag.speed * 10000)
		{
			bag.fixPos();
			const float k = bag.speed * elapsedTime / Helper::getDist(bag.getPosition(), bag.movePosition);
			Vector2f newPos = bag.getPosition();
			if (bag.movePosition.x != 0)
				newPos.x = bag.getPosition().x + (bag.movePosition.x - bag.getPosition().x) * k;
			if (bag.movePosition.y != 0)
				newPos.y = bag.getPosition().y + (bag.movePosition.y - bag.getPosition().y) * k;
			bag.setPosition(newPos);
			bag.fixCells();
		}
		else
			bag.movePosition = Vector2f(0, 0);
		//----------------

		// bag selection
		if (bag.currentState == bagClosed)
		{
			const Vector2f selectionPos = Vector2f(bag.getPosition().x + bag.selectionZoneClosedOffset.x, bag.getPosition().y + bag.selectionZoneClosedOffset.y);
			bag.readyToChangeState = (Helper::getDist(mousePos, selectionPos) <= bag.selectionZoneRadiusClosed);
		} else
		if (bag.currentState == bagOpen)
		{
			const Vector2f selectionPos = Vector2f(bag.getPosition().x + bag.selectionZoneOpenOffset.x, bag.getPosition().y + bag.selectionZoneOpenOffset.y);
			bag.readyToChangeState = (Helper::getDist(mousePos, selectionPos) <= bag.selectionZoneRadiusOpen);
		}
		//--------------
	}

	// nearest bag
	if (currentMovingBag == -1)
	{
		for (int cntForDist = 0; cntForDist < boundBags->size(); cntForDist++)
		{
			if (Helper::getDist(mousePos, boundBags->at(cntForDist).getPosition()) <= minDistToClosed && boundBags->at(cntForDist).currentState == bagClosed)
			{
				minDistToClosed = Helper::getDist(mousePos, boundBags->at(cntForDist).getPosition());
				minDistToClosedIndex = cntForDist;
			}
			if (Helper::getDist(mousePos, boundBags->at(cntForDist).getPosition()) <= minDistToOpen && boundBags->at(cntForDist).currentState == bagOpen)
			{
				minDistToOpen = Helper::getDist(mousePos, boundBags->at(cntForDist).getPosition());
				minDistToOpenIndex = cntForDist;
			}
		}		
	}
	//------------

	if (heldItem.content.first != Tag::emptyCell)
	{
		const Vector2f shiftVector = Vector2f(Mouse::getPosition().x - heldItem.position.x, Mouse::getPosition().y - heldItem.position.y);
		heldItem.position.x += shiftVector.x; heldItem.position.y += shiftVector.y;
	}

	uiEffectsSystem.interact(elapsedTime);
}

void InventoryMaker::crashIntoOtherBags(int cnt)
{
	int lapsCount = 20;
	while (true)
	{
		bool isBreak = true;
		for (auto& anotherBag : *boundBags)
		{
			if (anotherBag.getPosition() == boundBags->at(cnt).getPosition())
				continue;
			const Vector2f newPos = Vector2f(boundBags->at(cnt).getPosition().x + boundBags->at(cnt).shiftVector.x, boundBags->at(cnt).getPosition().y + boundBags->at(cnt).shiftVector.y);

			if (Helper::getDist(newPos, anotherBag.getPosition()) < boundBags->at(cnt).getRadius() + anotherBag.getRadius())
			{
				if (boundBags->at(cnt).currentState == bagClosed)
				{
					float k = 0.05 * lapsCount * ((boundBags->at(cnt).getRadius() + anotherBag.getRadius()) - Helper::getDist(newPos, anotherBag.getPosition())) / Helper::getDist(newPos, anotherBag.getPosition());
					boundBags->at(cnt).shiftVector = Vector2f((newPos.x - anotherBag.getPosition().x) * k, (newPos.y - anotherBag.getPosition().y) * k);
					isBreak = false;
					break;
				}
			}
		}
		if (isBreak)
			break;
		lapsCount++;
	}

	Vector2f newPos = Vector2f(boundBags->at(cnt).getPosition().x + boundBags->at(cnt).shiftVector.x, boundBags->at(cnt).getPosition().y + (boundBags->at(cnt).shiftVector.y));
	boundBags->at(cnt).movePosition = newPos;
	boundBags->at(cnt).shiftVector.x = 0; boundBags->at(cnt).shiftVector.y = 0;
}

void InventoryMaker::onMouseUp()
{
	usedMouse = false;
	const Vector2f mousePos = Vector2f(Mouse::getPosition());

	if (pickedCell != nullptr)	
		HeroBag::putItemIn(pickedCell, boundBags);	

	int cnt = -1;
	for (auto& bag : *boundBags)
	{
		cnt++;
		// crash into other bags
		if (bag.currentState == bagClosed && bag.wasMoved)
		{
			crashIntoOtherBags(cnt);
		}
		//----------------------

		// bag state changing
		if (cnt == currentMovingBag || currentMovingBag == -1)
		{
			usedMouse = true;
			if (bag.readyToEject)
				bag.currentState = ejected;
			if (bag.currentState != bagOpen)
			{
				if (bag.currentState == bagClosed && bag.readyToChangeState && !bag.wasMoved)
				{
					bag.currentState = bagOpening;
					bag.wasMoved = false;
					continue;
				}
			}
			else
			{
				if (bag.currentState == bagOpen && bag.readyToChangeState)
				{
					bag.currentState = bagClosing;
					bag.wasMoved = false;
					continue;
				}
			}
		}
		bag.wasMoved = false;
		if (bag.currentState != bagOpen)
			continue;
		//-------------------

		// put cursor item to bag
		if (heldItem.content.first != Tag::emptyCell)
		{
			int curIndex = bag.getSelectedCell(mousePos);
			if (curIndex == -1)
				continue;
			auto& item = bag.cells[curIndex];
			if (item.content.first == Tag::emptyCell || item.content.first == heldItem.content.first)
			{
				item.content.first = heldItem.content.first;
				item.content.second += heldItem.content.second;
				if (item.content.second > HeroBag::itemsMaxCount.at(Tag(item.content.first)))
				{
					heldItem.content.second = item.content.second % HeroBag::itemsMaxCount.at(Tag(item.content.first));
					item.content.second = HeroBag::itemsMaxCount.at(Tag(item.content.first));
				}
				else
					heldItem.content = { Tag::emptyCell, 0 };
				break;
			}
			else
			{
				const std::pair<Tag, int> temp = heldItem.content;
				heldItem.content = item.content;
				item.content = temp;
			}
		}
		else
		{
			int curIndex = bag.getSelectedCell(mousePos);
			if (curIndex != -1)
			{
				heldItem.content = bag.cells[curIndex].content;
				heldItem.position = bag.cells[curIndex].position;
				bag.cells[curIndex].content = { Tag::emptyCell, 0 };
			}
		}
		//-----------------------
	}
	currentMovingBag = -1;
}

void InventoryMaker::drawHeroInventory(float elapsedTime, RenderWindow& window)
{
	usedMouse = false;
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	// draw bags
	int cnt = -1;
	bool cursorTurnedOn = false;
	//cursorText = "";
	bagPosDot.setPosition(0, 0);
	for (auto& bag : *boundBags)
	{
		cnt++;
		bag.draw(&window, elapsedTime);
		bag.readyToEject = false;
		if (bag.wasMoved)
			currentMovingBag = cnt;
		int curIndex = bag.getSelectedCell(mousePos);

		if (bag.readyToChangeState || heldItem.content.first != Tag::emptyCell || curIndex != -1)
			usedMouse = true;

		// dropping bag
		if (Helper::getDist(bag.getPosition(), Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2)) <= dropZoneRadius && bag.currentState == bagClosed)
		{
			cursorTurnedOn = true;
			if (cursorText.empty())
			{
				uiEffectsSystem.addEffect(UIEffects::transparencyRemoval, &dropZone, "dropZone", 3 * 10e4);
			}
			cursorText = "  throw away";
			cursorTextPos = bag.getPosition();
			bag.readyToEject = true;
			bagPosDot.setPosition(bag.getPosition());
		}
		//-------------

		if (cnt == currentMovingBag)
			bagPosDot.setPosition(bag.getPosition());

		// drawing bag content
		if (bag.currentState != bagOpen)
			continue;
		for (int cnt2 = 0; cnt2 < bag.cells.size(); cnt2++)
		{
			auto item = bag.cells[cnt2];
			//drawing cell background
			if (bag.getSelectedCell(Vector2f(Mouse::getPosition())) == cnt2)
			{
				const Vector2f backgroundOffset = cellsSpriteList.at(Tag::emptyCell).offset;
				selectedCellBackground->setPosition(item.position.x - HeroBag::itemCommonRadius - backgroundOffset.x, item.position.y - HeroBag::itemCommonRadius - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
			//-----------------------

			if (bag.cells[cnt2].content.first == Tag::emptyCell)
				continue;

			auto sprite = cellsSpriteList.at(Tag(item.content.first)).sprite;
			sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
			const Vector2f offset = cellsSpriteList.at(Tag(item.content.first)).offset;
			sprite.setPosition(Vector2f(item.position.x - HeroBag::itemCommonRadius - offset.x, item.position.y - HeroBag::itemCommonRadius - offset.y));
			sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
			window.draw(sprite);

			if (HeroBag::itemsMaxCount.at(item.content.first) != 1)
				textWriter.drawNumberOfItems(sprite.getPosition(), item.content.second, window);
		}
		//--------------------
	}
	//----------

	//drawing held item
	if (heldItem.content.first != Tag::emptyCell)
	{
		auto sprite = cellsSpriteList.at(Tag(heldItem.content.first)).sprite;
		sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
		sprite.setPosition(Vector2f(heldItem.position.x - HeroBag::itemCommonRadius, heldItem.position.y - HeroBag::itemCommonRadius));
		window.draw(sprite);
		if (HeroBag::itemsMaxCount.at(heldItem.content.first) != 1)
			textWriter.drawNumberOfItems(sprite.getPosition(), heldItem.content.second, window);
	}
	//-----------------

	// draw cursor text
	if (!cursorTurnedOn)
	{
		cursorText = "";
		uiEffectsSystem.resetEffects({ "dropZone" });
	}
	if (cursorText == "  throw away")
	{
		textWriter.drawString(cursorText, NormalFont, 35 * Helper::GetScreenSize().y / 1440, cursorTextPos.x - Helper::GetScreenSize().x / 26, cursorTextPos.y - Helper::GetScreenSize().y / 30,
			&window, Color(53, 53, 53, 255));
		window.draw(dropZone);
	}
	if (bagPosDot.getPosition() != Vector2f(0, 0))
		window.draw(bagPosDot);
}

void InventoryMaker::drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window)
{
	for (int i = 0; i < inventory->size(); i++)
		if (inventory->at(i).first == Tag::emptyCell || inventory->at(i).second == 0)
			inventory->erase(inventory->begin() + i);

	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const FloatRect cellRect = cellsSpriteList.at(Tag::chamomile).sprite.getGlobalBounds();	
	int maxInRaw;
	if (inventory->size() > 3)
		maxInRaw = sqrt(inventory->size());
	else
		maxInRaw = inventory->size();
	int raw = 0, column = 0;
	while (raw * maxInRaw + column < inventory->size())
	{		
		//drawing cell background		
		if (!(mousePos.x < position.x || mousePos.y < position.y))
			if (int(mousePos.x - position.x) / int(cellRect.width) == column && 
				int(mousePos.y - position.y) / int(cellRect.height) == raw)
			{
				pickedCell = &(inventory->at(raw * maxInRaw + column));
				const Vector2f backgroundOffset = cellsSpriteList.at(Tag::emptyCell).offset;
				selectedCellBackground->setPosition(position.x + column * cellRect.width - backgroundOffset.x, position.y + raw * cellRect.height - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
		//-----------------------

		if (inventory->at(raw * maxInRaw + column).first != Tag::emptyCell)
		{
			if (cellsSpriteList.count(Tag(inventory->at(raw * maxInRaw + column).first)) > 0)
			{
				auto sprite = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).sprite;
				sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
				const Vector2f offset = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).offset;
				sprite.setPosition(Vector2f(position.x + column * cellRect.width - offset.x, position.y + raw * cellRect.width - offset.y));
				sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
				window.draw(sprite);

				if (HeroBag::itemsMaxCount.at(inventory->at(raw * maxInRaw + column).first) != 1)
					textWriter.drawNumberOfItems(sprite.getPosition(), inventory->at(raw * maxInRaw + column).second, window);
			}
		}
		column++;
		if (column >= maxInRaw)
		{
			raw++;
			column = 0;
		}
	}
}

void InventoryMaker::resetAnimationValues()
{
	animationCounter = 0;
	timeAfterAnimationEffect = 0;
}