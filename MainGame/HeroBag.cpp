#include "HeroBag.h"
#include "GridList.h"

HeroBag::HeroBag()
{
	stateChangeTime = 100000;
}

HeroBag::~HeroBag()
{
}

std::unordered_map<Tag, int> HeroBag::itemsMaxCount = std::unordered_map<Tag, int>();
float HeroBag::itemCommonRadius = Helper::GetScreenSize().y / 36;
std::string HeroBag::textureCB = "Game/inventorySprites/bag1Icon.png";
std::string HeroBag::textureCBS = "Game/inventorySprites/bag1IconHover.png";
std::string HeroBag::textureOB = "Game/inventorySprites/bag1.png";
std::string HeroBag::textureOBS = "Game/inventorySprites/bag1Hover.png";
std::vector<std::pair<Tag, int>> HeroBag::testInventory = 
{ {Tag::yarrow, 2}, {Tag::yarrow, 3}, {Tag::emptyCell, 0}, {Tag::inkyBlackPen, 1}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0} };
std::vector<std::pair<Tag, int>> HeroBag::emptyInventory =
{ {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0} };

void HeroBag::initialize(Vector2f position, Vector2f sizeClosed, Vector2f sizeOpen, bool isSelectable, std::vector<std::pair<Tag, int>> inventory, std::string textureClosedBagPath, std::string textureClosedBagSelectedPath, std::string textureOpenBagPath, std::string textureOpenBagSelectedPath)
{
	this->textureClosedBag.loadFromFile(textureClosedBagPath);
	this->textureClosedBagSelected.loadFromFile(textureClosedBagSelectedPath);
	this->textureOpenBag.loadFromFile(textureOpenBagPath);
	this->textureOpenBagSelected.loadFromFile(textureOpenBagSelectedPath);
	
	this->position = position;
	lastMousePos = position;

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

	if (inventory.empty())
		inventory = emptyInventory;
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.1f, position.y + sizeOpen.y * -0.15f), inventory[0], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.135f, position.y + sizeOpen.y * -0.15f), inventory[1], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.2f, position.y + sizeOpen.y * 0.014f), inventory[2], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.018f, position.y + sizeOpen.y * 0.0f), inventory[3], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.241f, position.y + sizeOpen.y * 0.004f), inventory[4], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * -0.08f, position.y + sizeOpen.y * 0.150f), inventory[5], itemCommonRadius));
	cells.push_back(createCell(Vector2f(position.x + sizeOpen.x * 0.131f, position.y + sizeOpen.y * 0.150f), inventory[6], itemCommonRadius));
}

Cell HeroBag::createCell(Vector2f position, std::pair<Tag, int> content, float radius)
{
	Cell cell;
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

float HeroBag::getRadius()
{
	if (currentState == bagClosed)
		return spriteClosedBag.getGlobalBounds().width / 2;
	if (currentState == bagOpening)
		return spriteClosedBagSelected.getGlobalBounds().width / 2;
	if (currentState == bagOpen)
		return spriteOpenBag.getGlobalBounds().width / 2;
	return 0;
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

std::vector<std::pair<Tag, int>> HeroBag::cellsToInventory(std::vector<Cell> cells)
{
	std::vector<std::pair<Tag, int>> ans;
	for (auto&cell : cells)	
		ans.push_back({ cell.content.first, cell.content.second });
	return ans;
}

bool HeroBag::canAfford(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem)
{
	for (auto&item : recipe)
	{
		if (heldItem != nullptr)
		{
			if (item.first == heldItem->content.first)
				item.second -= heldItem->content.second;
		}
		for (auto&bag : *bags)
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

void HeroBag::takeItems(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem)
{
	for (auto&item : recipe)
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
			heldItem->content.first = Tag::emptyCell;
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
						bag.cells[cnt].content.first = Tag::emptyCell;
					}
				}
			if (isBreak)
				break;
		}
	}
}

bool HeroBag::putItemsIn(std::vector<std::pair<Tag, int>> loot, std::vector<HeroBag>* bags)
{
	bool result = true;
	for (auto&item : loot)
	{		
		for (auto& bag : *bags)
		{			
			bool isBreak = false;
			for (int cnt = 0; cnt < bag.cells.size(); cnt++)
				if (item.first == bag.cells[cnt].content.first || bag.cells[cnt].content.first == Tag::emptyCell)
				{
					if (item.second <= (HeroBag::itemsMaxCount[item.first] - bag.cells[cnt].content.second))
					{
						bag.cells[cnt].content.second += item.second;
						bag.cells[cnt].content.first = item.first;
						item = std::make_pair(Tag::emptyCell, 0);
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

void HeroBag::draw(RenderWindow* window, float elapsedTime, bool canBeMoved)
{
	Vector2f screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);			
	if (currentState == bagClosed && (readyToChangeState || wasMoved) && canBeMoved)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			shiftVector = { 0, 0 };
			if (lastMousePos != Vector2f(0, 0))
				shiftVector = Vector2f(Mouse::getPosition().x - lastMousePos.x, Mouse::getPosition().y - lastMousePos.y);
			changeCellsPosition(shiftVector);
			position.x += shiftVector.x; position.y += shiftVector.y;
			if (shiftVector != Vector2f(0, 0))
				wasMoved = true;
		}
	}

	lastMousePos = Vector2f(Mouse::getPosition());

	if (currentState == bagOpen)
	{	
		textureOpenOffset = Vector2f(sizeOpen.x / 2, sizeOpen.y / 1.7);
		if (readyToChangeState)
		{			
			spriteOpenBagSelected.setPosition(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y);
			window->draw(spriteOpenBagSelected);
		}
		else
		{			
			spriteOpenBag.setPosition(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y);
			window->draw(spriteOpenBag);
		}			
		return;
	}

	if (currentState == bagClosed)
	{
		textureClosedOffset = Vector2f(sizeClosed.x / 2, sizeClosed.y / 1.7);
		if (readyToChangeState)
		{			
			spriteClosedBagSelected.setPosition(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y);
			window->draw(spriteClosedBagSelected);
		}
		else
		{			
			spriteClosedBag.setPosition(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y);
			window->draw(spriteClosedBag);
		}		
		return;
	}

	if (currentState == bagOpening)
	{		
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagOpen;
			stateChangingTime = 0;
		}
		else
		{
			const Vector2f toCenterVector = Vector2f(screenCenter.x - position.x, screenCenter.y - position.y);
			const float cutCoefficient = sqrt(pow((sizeOpen.x - sizeClosed.x) / 2 * elapsedTime / stateChangeTime, 2) + pow((sizeOpen.y - sizeClosed.y) / 2 * elapsedTime / stateChangeTime, 2)) /
				sqrt(pow(toCenterVector.x, 2) + pow(toCenterVector.y, 2));
			position.x += toCenterVector.x * cutCoefficient; position.y += toCenterVector.y * cutCoefficient;

			const Vector2f scaleValue = { (sizeClosed.x + ((sizeOpen.x - sizeClosed.x) * stateChangingTime / stateChangeTime)) / textureOpenBag.getSize().x, (sizeClosed.y + ((sizeOpen.y - sizeClosed.y) * stateChangingTime / stateChangeTime)) / textureOpenBag.getSize().y };
			spriteOpenBag.setScale(scaleValue);

			textureClosedOffset = Vector2f(spriteOpenBag.getGlobalBounds().width / 2, spriteOpenBag.getGlobalBounds().height / 1.7);
			spriteOpenBag.setPosition(position.x - textureClosedOffset.x, position.y - textureClosedOffset.y);

			changeCellsPosition(Vector2f(toCenterVector.x * cutCoefficient, toCenterVector.y * cutCoefficient));			
		}
		window->draw(spriteOpenBag);
		return;
	}

	if (currentState == bagClosing)
	{
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagClosed;
			stateChangingTime = 0;
		}
		else
		{
			const Vector2f fromCenterVector = Vector2f(position.x - screenCenter.x, position.y - screenCenter.y);
			const float cutCoefficient = sqrt(pow((sizeOpen.x - sizeClosed.x) / 2 * elapsedTime / stateChangeTime, 2) + pow((sizeOpen.y - sizeClosed.y) / 2 * elapsedTime / stateChangeTime, 2)) /
				sqrt(pow(fromCenterVector.x, 2) + pow(fromCenterVector.y, 2));
			position.x += fromCenterVector.x * cutCoefficient; position.y += fromCenterVector.y * cutCoefficient;

			const Vector2f scaleValue = { (sizeClosed.x + ((sizeOpen.x - sizeClosed.x) * (stateChangeTime - stateChangingTime) / stateChangeTime)) / textureClosedBag.getSize().x, (sizeClosed.y + ((sizeOpen.y - sizeClosed.y) * (stateChangeTime - stateChangingTime) / stateChangeTime)) / textureClosedBag.getSize().y };
			spriteClosedBag.setScale(scaleValue);

			textureOpenOffset = Vector2f(spriteClosedBag.getGlobalBounds().width / 2, spriteClosedBag.getGlobalBounds().height / 1.7);
			spriteClosedBag.setPosition(position.x - textureOpenOffset.x, position.y - textureOpenOffset.y);

			changeCellsPosition(Vector2f(fromCenterVector.x * cutCoefficient, fromCenterVector.y * cutCoefficient));			
		}
		window->draw(spriteClosedBag);
		return;
	}
}