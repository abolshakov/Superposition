#include "MushroomStone.h"
#include "Helper.h"

MushroomStone::MushroomStone(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	animationSpeed = 0.0004f;
	animationLength = 15;
	this->typeOfObject = typeOfObject;
	radius = 200;
	toSaveName = "spawn";
	setType(typeOfObject);
}

void MushroomStone::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (800, 800);
}

Vector2i MushroomStone::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void MushroomStone::initPedestal()
{
	focus1 = Vector2f (textureBox.left + int(textureBox.width / 3.2), int(position.y));
	focus2 = Vector2f (textureBox.left + textureBox.width - int(textureBox.width / 4.4), int(position.y));
	ellipseSize = float((focus2.x - focus1.x) * 1.09);
}

Vector2f MushroomStone::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int MushroomStone::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void MushroomStone::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement bonfireBody;
	bonfireBody.path = "Game/worldSprites/terrainObjects/mushroomStone/t1_" + std::to_string(currentSprite[0]) + ".png";
	bonfireBody.size = Vector2f(conditionalSizeUnits);
	bonfireBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(bonfireBody);
}