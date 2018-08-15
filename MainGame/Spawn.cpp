#include "Spawn.h"
#include "Helper.h"

Spawn::Spawn(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 200;
	toSaveName = "spawn";
	setType(typeOfObject);
}

void Spawn::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(1000, 1000);
}

Vector2i Spawn::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.6));
}

void Spawn::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.8), int(position.y));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 3.8), int(position.y));
		ellipseSize = int((focus2.x - focus1.x) * 1.08);
	}
	if (typeOfObject == 2)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 2.3), int(position.y));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 3.3), int(position.y));
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
	if (typeOfObject == 3)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.5), int(position.y + textureBox.height / 20));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 2), int(position.y + textureBox.height / 100));
		ellipseSize = int((focus2.x - focus1.x) * 1.3);
	}
}

std::string Spawn::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/spawn/spawn" + std::to_string(typeOfObject) + ".png";
}