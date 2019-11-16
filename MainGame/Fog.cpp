#include "Fog.h"
#include "Helper.h"
#include "cmath" 
Fog::Fog(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	state = common;
	tag = Tag::fog;
	toSaveName = "fog";
	intangible = true;
	currentSprite[0] = rand() % 150 + 1;
	Fog::setType(typeOfObject);
}

void Fog::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i(1540, 800);
}

Vector2i Fog::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width) * getScaleRatio().x);
	textureBox.height = int(float(textureBox.height) *getScaleRatio().y);
	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

void Fog::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = position;
		focus2 = position;
		ellipseSize = 0;
	}
}

Vector2f Fog::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Fog::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Fog::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement fogBody;
	fogBody.size = Vector2f(conditionalSizeUnits);
	fogBody.offset = Vector2f(textureBoxOffset);

	switch (state)
	{
	case absorbed:
		animationLength = 150;
		animationSpeed = 0.0010f;
		fogBody.path = "Game/worldSprites/terrainObjects/fog/" + std::to_string(currentSprite[0]) + ".png";
		fogBody.color.a = color.a;
		if (fogBody.color.a == 0)
			deletePromiseOn();
		break;
	default:
		animationLength = 150;
		animationSpeed = 0.0010f;
		fogBody.path = "Game/worldSprites/terrainObjects/fog/" + std::to_string(currentSprite[0]) + ".png";
		break;
	}	
	
	additionalSprites.push_back(fogBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		if (state == absorbed)
		{
			if (color.a >= 3)
				color.a -= 3;
			else
				color.a = 0;
		}
		timeForNewSprite = 0;
		if (++currentSprite[0] > animationLength)
		{
			currentSprite[0] = 1;
		}
	}
}