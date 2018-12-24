#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "grass";
	setType(typeOfObject);
	tag = grassTag;
}

void Grass::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (150, 150);
}

Vector2i Grass::calculateTextureOffset()
{	
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	
	if (typeOfObject == 0)
		return Vector2i (0, 0);	
	
	return Vector2i (textureBox.width / 2, int(textureBox.height / 2));
}

Vector2f Grass::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Grass::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::string Grass::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/grass/grass" + std::to_string(typeOfObject) + ".png";
}