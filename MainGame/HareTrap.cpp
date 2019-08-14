#include "HareTrap.h"
#include "Helper.h"

HareTrap::HareTrap(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	toSaveName = "hareTrap";
	setType(typeOfObject);
	currentSprite[0] = 1;
	id = Tag::hareTrap;
	tag = Tag::hareTrap;
	inventory = HeroBag::emptyInventory;
}

void HareTrap::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (140, 111);
}

Vector2i HareTrap::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void HareTrap::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x, position.y);
		focus2 = Vector2f (position.x, position.y);
		ellipseSize = float((focus2.x - focus1.x));
	}
}

Vector2f HareTrap::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f HareTrap::getEnterPosition() const
{
	return {position.x - conditionalSizeUnits.x / 2.0f, position.y + conditionalSizeUnits.y / 10.0f };
}

int HareTrap::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void HareTrap::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement trapBody;
	trapBody.path = "Game/worldSprites/terrainObjects/hareTrap/hareTrap" + std::to_string(currentSprite[0]) + ".png";
	trapBody.size = Vector2f(conditionalSizeUnits);
	trapBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(trapBody);
}