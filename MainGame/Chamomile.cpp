#include "Chamomile.h"
#include "Helper.h"

Chamomile::Chamomile(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "chamomile";
	setType(typeOfObject);
	tag = Tag::chamomile;

	count = 1;
	id = Tag::chamomile;	
}

void Chamomile::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(297, 263);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(279, 281);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(313, 264);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(223, 267);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(203, 206);
}

Vector2i Chamomile::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void Chamomile::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x - textureBox.width / 10, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 10, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.08);
	}
}

Vector2f Chamomile::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Chamomile::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Chamomile::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
    additionalSprites.clear();
	spriteChainElement body;
	body.path = "Game/worldSprites/terrainObjects/chamomile/chamomile" + std::to_string(typeOfObject) + ".png";
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);
}