#include "Mugwort.h"
#include "Helper.h"

Mugwort::Mugwort(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	toSaveName = "mugwort";
	setType(typeOfObject);
	radius = 50;
	tag = mugwortTag;

	count = 1;
	id = 4;
	maxCount = 4;
}

void Mugwort::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(308, 371);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(303, 325);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(250, 271);
}

Vector2i Mugwort::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void Mugwort::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2f(position.x + textureBox.width / 10, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.08);
	}
}

Vector2f Mugwort::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Mugwort::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Mugwort::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement mugwortBody;
	mugwortBody.path = "Game/worldSprites/terrainObjects/mugwort/mugwort" + std::to_string(typeOfObject) + ".png";
	mugwortBody.size = Vector2f(conditionalSizeUnits);
	mugwortBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(mugwortBody);
}