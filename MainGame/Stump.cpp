#include "Stump.h"
#include "Helper.h"

Stump::Stump(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 4;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 150;
	animationSpeed = 0.0005f;
	toSaveName = "stump";
	setType(typeOfObject);
	tag = stumpTag;
}

void Stump::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (424, 304);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (308, 196);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (212, 172);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (660, 312);
}

Vector2i Stump::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	if (typeOfObject == 1)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 2)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 3)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 4)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));

	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.25));
}

void Stump::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x - textureBox.width / 2, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 3, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.2);
	}
	else
		if (typeOfObject == 2)
		{
			focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
			focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
			ellipseSize = float((focus2.x - focus1.x) * 1.6);
		}
		else
			if (typeOfObject == 3)
			{
				focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
				focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
				ellipseSize = float((focus2.x - focus1.x) * 1.6);
			}
			else
				if (typeOfObject == 4)
				{
					focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
					focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
					ellipseSize = float((focus2.x - focus1.x) * 1.6);
				}
}

Vector2f Stump::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Stump::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Stump::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement stumpBody;
	stumpBody.size = Vector2f(conditionalSizeUnits);
	stumpBody.offset = Vector2f(textureBoxOffset);

	switch (state)
	{
		case common:
		{
			animationLength = 1;
			stumpBody.path = "Game/worldSprites/terrainObjects/stump/stump" + std::to_string(typeOfObject) + ".png";
			break;
		}
		case absorbed:
		{
			animationLength = 15;
			stumpBody.path = "Game/worldSprites/terrainObjects/stump/stump" + std::to_string(typeOfObject) + ".png";
			transparency = 100 - currentSprite[0] * 100 / animationLength;
			break;
		}
	}

	additionalSprites.push_back(stumpBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				deletePromiseOn();
			}
			currentSprite[0] = 1;
		}
	}
}