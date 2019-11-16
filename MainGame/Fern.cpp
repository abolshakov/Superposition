#include "Fern.h"
#include "Helper.h"

Fern::Fern(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	toSaveName = "fern";
	setType(typeOfObject);
	tag = Tag::fern;

	count = 1;
	id = Tag::fern;
}

void Fern::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(330, 370);
}

Vector2i Fern::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.7));
}

void Fern::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2f(position.x + textureBox.width / 10, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.08);
	}
}

Vector2f Fern::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f Fern::getEnterPosition() const
{
	return { position.x, position.y + 1 };
}

int Fern::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Fern::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement body;
	body.path = "Game/worldSprites/terrainObjects/fern/fern" + std::to_string(typeOfObject) + ".png";
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);
}