#include "Ground.h"
#include "Helper.h"

Ground::Ground(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 4;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "ground";
	setType(typeOfObject);
	tag = forestTreeTag;
}

void Ground::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (1000, 1000);
	this->zCoord = typeOfObject * 10 + 5;
}

Vector2i Ground::calculateTextureOffset()
{
	return {0, 0};
}

Vector2f Ground::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Ground::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Ground::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement groundBody;
	groundBody.path = "Game/worldSprites/terrainObjects/ground/ground" + std::to_string(typeOfObject) + ".png";
	groundBody.size = Vector2f(conditionalSizeUnits);
	groundBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(groundBody);
}