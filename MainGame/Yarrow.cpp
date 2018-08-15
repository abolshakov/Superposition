#include "Yarrow.h"
#include "Helper.h"

Yarrow::Yarrow(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	toSaveName = "yarrow";
	setType(typeOfObject);
	tag = yarrowTag;

	count = 1;
	id = 4;
	maxCount = 4;
}

void Yarrow::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(200, 200);
}

Vector2i Yarrow::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void Yarrow::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 10, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.08);
	}
}

std::string Yarrow::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/yarrow/yarrow" + std::to_string(typeOfObject) + ".png";
}