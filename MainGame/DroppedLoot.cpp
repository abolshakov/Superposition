#include "DroppedLoot.h"
#include "Helper.h"

DroppedLoot::DroppedLoot(std::string objectName, Vector2f centerPosition, int typeOfObject, int count) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "drop";
	setType(typeOfObject);
	radius = Helper::GetScreenSize().y / 36;
	tag = droppedLootTag;
	this->count = count;
}

void DroppedLoot::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	id = lootItemsIdList(typeOfObject);
	conditionalSizeUnits = Vector2i(radius * 3, radius * 3);
}

Vector2i DroppedLoot::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void DroppedLoot::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 10, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 10, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.08);
}

Vector2f DroppedLoot::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int DroppedLoot::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void DroppedLoot::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement sprite;
	switch (typeOfObject)
	{
		case 1:
			sprite.path = "Game/inventorySprites/chamomile";
			break;
		case 3:
			sprite.path = "Game/inventorySprites/chamomile";
			break;
		case 4:
			sprite.path = "Game/inventorySprites/yarrow";
			break;
		default:
			sprite.path = "Game/inventorySprites/bagCell";
			break;
	}
	if (this->isSelected) sprite.path += "L";
	sprite.path += ".png";
	sprite.size = Vector2f(conditionalSizeUnits);
	sprite.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(sprite);
}