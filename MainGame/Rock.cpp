#include "Rock.h"
#include "Helper.h"

Rock::Rock(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 6;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 0.0005f;
	toSaveName = "rock";
	setType(typeOfObject);
	tag = rockTag;
}

void Rock::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (272, 80);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (388, 228);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (568, 292);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (336, 240);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i (280, 212);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i (272, 196);

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2i Rock::calculateTextureOffset()
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
	if (typeOfObject == 5)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 6)

	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.25));
}

void Rock::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.6);
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
				else
					if (typeOfObject == 5)
					{
						focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
						focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
						ellipseSize = float((focus2.x - focus1.x) * 1.6);
					}
					else
						if (typeOfObject == 6)
						{
							focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
							focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
							ellipseSize = float((focus2.x - focus1.x) * 1.6);
						}
						
	//textureBoxOffset = Vector2f ((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

Vector2f Rock::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Rock::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Rock::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement rockBody;
	rockBody.size = Vector2f(conditionalSizeUnits);
	rockBody.offset = Vector2f(textureBoxOffset);

	switch (state)
	{
		case common:
		{
			animationLength = 1;
			rockBody.path = "Game/worldSprites/terrainObjects/rock/rock" + std::to_string(typeOfObject) + ".png";
			break;
		}
		case absorbed:
		{
			animationLength = 15;
			rockBody.path = "Game/worldSprites/terrainObjects/rock/rock" + std::to_string(typeOfObject) + ".png";
			transparensy = 100 - currentSprite * 100 / animationLength;
			break;
		}
	}

	additionalSprites.push_back(rockBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				delatePromiseOn();
			}
			currentSprite = 1;
		}
	}
}