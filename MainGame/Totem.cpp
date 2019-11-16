#include "Totem.h"
#include "Helper.h"

Totem::Totem(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 20;
	toSaveName = "totem";
	setType(typeOfObject);
	tag = Tag::rock;
}

void Totem::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i(154, 544);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i(154, 544);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i(568, 292);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i(336, 240);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i(280, 212);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i(272, 196);

	radius = std::max(conditionalSizeUnits.x, conditionalSizeUnits.y) / 2;
}

Vector2i Totem::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	if (typeOfObject == 1)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.15));
	if (typeOfObject == 2)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.15));
	if (typeOfObject == 3)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 4)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 5)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
	if (typeOfObject == 6)

		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

void Totem::initPedestal()
{
	//if (typeOfObject == 1)
	{
		focus1 = Vector2f(position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f(position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.8);
	}
	
	//textureBoxOffset = Vector2f ((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

Vector2f Totem::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Totem::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Totem::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement body;
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);

	switch (state)
	{
		case common:
		{
			animationLength = 1;
			std::string totemName = "";
			switch (typeOfObject)
			{
			case 1:
				{
				totemName = "unknown";
				break;
				}
			case 2:
				{
				totemName = "hare";
				break;
				}
			default:
				totemName = "unknown";
			}

			body.path = "Game/worldSprites/terrainObjects/totems/" + totemName + ".png";
			break;
		}	
	}

	additionalSprites.push_back(body);

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