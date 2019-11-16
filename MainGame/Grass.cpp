#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	toSaveName = "grass";
	setType(typeOfObject);
	tag = Tag::grass;
}

void Grass::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(252, 208);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(192, 140);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(240, 192);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(244, 196);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(148, 156);
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

void Grass::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement grassBody;
	grassBody.path = "Game/worldSprites/terrainObjects/grass/grass" + std::to_string(typeOfObject) + ".png";
	grassBody.size = Vector2f(conditionalSizeUnits);
	grassBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(grassBody);
}