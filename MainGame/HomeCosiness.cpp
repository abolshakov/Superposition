#include "HomeCosiness.h"
#include "Helper.h"

HomeCosiness::HomeCosiness(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	isBackground = true;
	toSaveName = "homeCosiness";
	zCoord = 100;
	setType(typeOfObject);
}

void HomeCosiness::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (500, 170);
}

Vector2i HomeCosiness::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void HomeCosiness::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x - textureBox.width / 3, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 3, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.2);
	}
}

Vector2f HomeCosiness::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int HomeCosiness::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void HomeCosiness::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement bonfireBody;
	bonfireBody.path = "Game/worldSprites/terrainObjects/homeCosiness/homeCosiness" + std::to_string(typeOfObject) + ".png";
	bonfireBody.size = Vector2f(conditionalSizeUnits);
	bonfireBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(bonfireBody);
}