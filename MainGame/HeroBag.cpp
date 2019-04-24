#include "HeroBag.h"
#include "GridList.h"

HeroBag::HeroBag()
{
	stateChangeTime = 100000;
}

HeroBag::~HeroBag()
{
}

std::unordered_map<lootItemsIdList, int> HeroBag::itemsMaxCount = std::unordered_map<lootItemsIdList, int>();
float HeroBag::itemCommonRadius = Helper::GetScreenSize().y / 36;

void HeroBag::initialize(std::string textureClosedBagPath, std::string textureClosedBagSelectedPath, std::string textureOpenBagPath, std::string textureOpenBagSelectedPath, Vector2f position, Vector2f sizeClosed, Vector2f sizeOpen, bool isSelectable)
{	
	this->textureClosedBag.loadFromFile(textureClosedBagPath);
	this->textureClosedBagSelected.loadFromFile(textureClosedBagSelectedPath);
	this->textureOpenBag.loadFromFile(textureOpenBagPath);
	this->textureOpenBagSelected.loadFromFile(textureOpenBagSelectedPath);
	
	this->position = position;

	this->isSelectable = isSelectable;

	this->spriteClosedBag.setTexture(this->textureClosedBag);
	this->spriteClosedBagSelected.setTexture(this->textureClosedBagSelected);
	this->spriteOpenBag.setTexture(this->textureOpenBag);
	this->spriteOpenBagSelected.setTexture(this->textureOpenBagSelected);
	this->sizeClosed = sizeClosed;
	this->sizeOpen = sizeOpen;
	this->selectionZoneRadiusClosed = sizeClosed.x / 2;
	this->selectionZoneRadiusOpen = sizeOpen.x / 5;

	this->spriteClosedBag.setScale(sizeClosed.x / this->textureClosedBag.getSize().x, sizeClosed.y / this->textureClosedBag.getSize().y);
	this->spriteOpenBag.setScale(sizeOpen.x / this->textureOpenBag.getSize().x, sizeOpen.y / this->textureOpenBag.getSize().y);
	this->spriteClosedBagSelected.setScale(sizeClosed.x / this->textureClosedBagSelected.getSize().x, sizeClosed.y / this->textureClosedBagSelected.getSize().y);
	this->spriteOpenBagSelected.setScale(sizeOpen.x / this->textureOpenBagSelected.getSize().x, sizeOpen.y / this->textureOpenBagSelected.getSize().y);

	this->textureClosedOffset = Vector2f(sizeClosed.x / 2, sizeClosed.y / 1.7);
	this->textureOpenOffset = Vector2f(sizeOpen.x / 2, sizeOpen.y / 1.7);

	this->spriteClosedBag.setPosition(Vector2f(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y));
	this->spriteClosedBagSelected.setPosition(Vector2f(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y));
	this->spriteOpenBag.setPosition(Vector2f(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y));
	this->spriteOpenBagSelected.setPosition(Vector2f(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y));

	this->selectionZoneClosedOffset = Vector2f(0, 0);
	this->selectionZoneOpenOffset = Vector2f(0, -textureOpenOffset.y + sizeOpen.y * 0.2f);

	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.1f, position.y + sizeOpen.y * -0.15f), std::make_pair(lootItemsIdList::chamomileFlower, 2), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.135f, position.y + sizeOpen.y * -0.15f), std::make_pair(lootItemsIdList::chamomileFlower, 3), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.2f, position.y + sizeOpen.y * 0.014f), std::make_pair(lootItemsIdList::noose, 1), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.018f, position.y + sizeOpen.y * 0.0f), std::make_pair(lootItemsIdList::chamomileFlower, 1), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.241f, position.y + sizeOpen.y * 0.004f), std::make_pair(lootItemsIdList::chamomileFlower,1), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.08f, position.y + sizeOpen.y * 0.150f), std::make_pair(lootItemsIdList::chamomileFlower, 1), itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.131f, position.y + sizeOpen.y * 0.150f), std::make_pair(lootItemsIdList::bagCell, 1), itemCommonRadius));
}

bagCell HeroBag::createCell(Vector2f position, std::pair<lootItemsIdList, int> content, float radius)
{
	bagCell cell;
	cell.position = position;
	cell.content = content;
	return cell;
}

int HeroBag::getSelectedCell(Vector2f position)
{
	for (int i = 0; i < cells.size(); i++)
	{
		if (Helper::getDist(position, cells[i].position) <= itemCommonRadius)
			return i;
	}
	return -1;
}

void HeroBag::drawCircuit(RenderWindow* window)
{
	for (int i = spriteOpenBag.getGlobalBounds().left; i <= spriteOpenBag.getGlobalBounds().left + spriteOpenBag.getGlobalBounds().width; i += 5)
	{
		for (int j = spriteOpenBag.getGlobalBounds().top; j <= spriteOpenBag.getGlobalBounds().top + spriteOpenBag.getGlobalBounds().height; j += 5)
		{
			if (getSelectedCell(Vector2f(i, j)) != -1)
			{
				RectangleShape rec;
				rec.setPosition(i, j);
				rec.setSize(Vector2f(5, 5));
				rec.setFillColor(Color::Red);
				window->draw(rec);
			}
		}
	}
}

void HeroBag::changeCellsPosition(Vector2f shift)
{
	for (auto& cell : cells)
	{
		cell.position.x += shift.x; cell.position.y += shift.y;
	}
}

bool HeroBag::canAfford(std::vector<std::pair<lootItemsIdList, int>> recipe, std::vector<HeroBag>* bags, bagCell* heldItem)
{
	for (auto item : recipe)
	{
		if (heldItem != nullptr)
		{
			if (item.first == heldItem->content.first)
				item.second -= heldItem->content.second;
		}
		for (auto bag : *bags)
		{
			if (item.second <= 0)
				break;
			for (int cnt = 0; cnt < bag.cells.size(); cnt++)
				if (item.first == bag.cells[cnt].content.first)
					item.second -= bag.cells[cnt].content.second;
		}
		if (item.second > 0)
			return false;
	}
	return true;
}

void HeroBag::takeItems(std::vector<std::pair<lootItemsIdList, int>> recipe, std::vector<HeroBag>* bags, bagCell* heldItem)
{
	for (auto item : recipe)
	{
		if (heldItem != nullptr)
		{
			if (item.second < heldItem->content.second)
			{
				heldItem->content.second -= item.second;
				item.second = 0;
				continue;
			}
			item.second -= heldItem->content.second;
			heldItem->content.second = 0;
			heldItem->content.first = lootItemsIdList::bagCell;
		}
		for (auto& bag : *bags)
		{
			bool isBreak = true;
			for (int cnt = 0; cnt < bag.cells.size(); cnt++)
				if (item.first == bag.cells[cnt].content.first)
				{
					if (item.second < bag.cells[cnt].content.second)
					{
						bag.cells[cnt].content.second -= item.second;
						item.second = 0;
						isBreak = true;
					}
					else
					{
						item.second -= bag.cells[cnt].content.second;
						bag.cells[cnt].content.second = 0;
						bag.cells[cnt].content.first = lootItemsIdList::bagCell;
					}
				}
			if (isBreak)
				break;
		}
	}
}

bool HeroBag::putItemsIn(std::vector<std::pair<lootItemsIdList, int>> loot, std::vector<HeroBag>* bags)
{
	bool result = true;
	for (auto item : loot)
	{		
		for (auto& bag : *bags)
		{			
			bool isBreak = false;
			for (int cnt = 0; cnt < bag.cells.size(); cnt++)
				if (item.first == bag.cells[cnt].content.first || bag.cells[cnt].content.first == lootItemsIdList::bagCell)
				{
					if (item.second <= (HeroBag::itemsMaxCount[item.first] - bag.cells[cnt].content.second))
					{
						bag.cells[cnt].content.second += item.second;
						bag.cells[cnt].content.first = item.first;
						item = std::make_pair(lootItemsIdList::bagCell, 0);
						isBreak = true;
						break;
					}
					else
					{
						item.second -= (HeroBag::itemsMaxCount[item.first] - bag.cells[cnt].content.second);
						bag.cells[cnt].content.second = HeroBag::itemsMaxCount[item.first];
						bag.cells[cnt].content.first = item.first;
					}
				}
			if (isBreak)
				break;
		}
		if (item.second != 0)
			result = false;
	}
	return result;
}

void HeroBag::draw(RenderWindow* window, float elapsedTime)
{
	Vector2f screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);
	Sprite *sprite = nullptr;
	
	if (currentState == bagClosed && (readyToChangeState || wasMoved))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Vector2f shiftVector = { 0, 0 };
			if (lastMousePos != Vector2f(0, 0))
				shiftVector = Vector2f(Mouse::getPosition().x - lastMousePos.x, Mouse::getPosition().y - lastMousePos.y);
			position.x += shiftVector.x; position.y += shiftVector.y;
			changeCellsPosition(shiftVector);
			if (shiftVector != Vector2f(0, 0))
				wasMoved = true;		
		}
	}
	lastMousePos = Vector2f(Mouse::getPosition());

	if (currentState == bagOpen)
	{	
		if (readyToChangeState)
			sprite = &spriteOpenBagSelected;
		else
			sprite = &spriteOpenBag;
		textureOpenOffset = Vector2f(sizeOpen.x / 2, sizeOpen.y / 1.7);
		sprite->setScale(sizeOpen.x / textureOpenBag.getSize().x, sizeOpen.y / textureOpenBag.getSize().y);
		sprite->setPosition(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y);
		window->draw(*sprite);	
		return;
	}

	if (currentState == bagClosed)
	{
		if (readyToChangeState)
			sprite = &spriteClosedBagSelected;
		else
			sprite = &spriteClosedBag;
		textureClosedOffset = Vector2f(sizeClosed.x / 2, sizeClosed.y / 1.7);
		sprite->setScale(sizeClosed.x / textureClosedBag.getSize().x, sizeClosed.y / textureClosedBag.getSize().y);
		sprite->setPosition(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y);
		window->draw(*sprite);
		return;
	}

	if (currentState == bagOpening)
	{
		sprite = &spriteClosedBagSelected;
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagOpen;
			stateChangingTime = 0.1f;
		}
		else
		{
			const Vector2f toCenterVector = Vector2f(screenCenter.x - position.x, screenCenter.y - position.y);
			const float cutCoefficient = sqrt(pow((sizeOpen.x - sizeClosed.x) * elapsedTime / stateChangeTime * (textureClosedOffset.x / sprite->getGlobalBounds().width), 2) + pow((sizeOpen.y - sizeClosed.y) * elapsedTime / stateChangeTime * (textureClosedOffset.y / sprite->getGlobalBounds().height), 2)) /
				sqrt(pow(toCenterVector.x, 2) + pow(toCenterVector.y, 2));
			position.x += toCenterVector.x * cutCoefficient; position.y += toCenterVector.y * cutCoefficient;

			const Vector2f scaleValue = { (sizeClosed.x + ((sizeOpen.x - sizeClosed.x) * stateChangingTime / stateChangeTime)) / textureClosedBag.getSize().x, (sizeClosed.y + ((sizeOpen.y - sizeClosed.y) * stateChangingTime / stateChangeTime)) / textureClosedBag.getSize().y };
			sprite->setScale(scaleValue);

			textureClosedOffset = Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 1.7);
			sprite->setPosition(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y);

			changeCellsPosition(Vector2f(toCenterVector.x * cutCoefficient, toCenterVector.y * cutCoefficient));
			window->draw(*sprite);			
		}
		return;
	}

	if (currentState == bagClosing)
	{
		sprite = &spriteOpenBagSelected;
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagClosed;
			stateChangingTime = 0.1f;
		}
		else
		{
			const Vector2f fromCenterVector = Vector2f(position.x - screenCenter.x, position.y - screenCenter.y);
			const float cutCoefficient = sqrt(pow((sizeOpen.x - sizeClosed.x) * elapsedTime / stateChangeTime * (textureOpenOffset.x / sprite->getGlobalBounds().width), 2) + pow((sizeOpen.y - sizeClosed.y) * elapsedTime / stateChangeTime * (textureOpenOffset.y / sprite->getGlobalBounds().height), 2)) /
				sqrt(pow(fromCenterVector.x, 2) + pow(fromCenterVector.y, 2));
			position.x += fromCenterVector.x * cutCoefficient; position.y += fromCenterVector.y * cutCoefficient;

			const Vector2f scaleValue = { (sizeClosed.x + ((sizeOpen.x - sizeClosed.x) * (stateChangeTime - stateChangingTime) / stateChangeTime)) / textureOpenBag.getSize().x, (sizeClosed.y + ((sizeOpen.y - sizeClosed.y) * (stateChangeTime - stateChangingTime) / stateChangeTime)) / textureOpenBag.getSize().y };
			sprite->setScale(scaleValue);

			textureOpenOffset = Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 1.7);
			sprite->setPosition(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y);

			changeCellsPosition(Vector2f(fromCenterVector.x * cutCoefficient, fromCenterVector.y * cutCoefficient));
			window->draw(*sprite);
		}
		return;
	}
}