#include "GroundConnection.h"
#include "Helper.h"

GroundConnection::GroundConnection(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 16;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "groundConnection";
	setType(typeOfObject);
	tag = Tag::groundConnection;
}

void GroundConnection::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if ((typeOfObject - 1) % 4 == 0)
	{
		this->conditionalSizeUnits = Vector2i (250, 1000);
	}
	else
		if ((typeOfObject - 1) % 4 == 1)
		{
			this->conditionalSizeUnits = Vector2i (250, 1000);
		}
		else
			if ((typeOfObject - 1) % 4 == 2)
			{
				this->conditionalSizeUnits = Vector2i (1000, 250);
			}
			else
				if ((typeOfObject - 1) % 4 == 3)
				{
					this->conditionalSizeUnits = Vector2i (1000, 250);
				}
	
	if (typeOfObject >= 1 && typeOfObject <= 4)
		this->zCoord = 1;
	else
		if (typeOfObject >= 5 && typeOfObject <= 8)
			this->zCoord = 2;
		else
			if (typeOfObject >= 9 && typeOfObject <= 12)
				this->zCoord = 3;
			else
				if (typeOfObject >= 13 && typeOfObject <= 16)
					this->zCoord = 4;
				else
					if (typeOfObject >= 17 && typeOfObject <= 20)
						this->zCoord = 5;
	this->zCoord *= 10;
}

Vector2i GroundConnection::calculateTextureOffset()
{
	if ((typeOfObject - 1) % 4 == 0)
		return Vector2i (textureBox.width, 0);
	else
		if ((typeOfObject - 1) % 4 == 1)
			return Vector2i (0, 0);
		else
			if ((typeOfObject - 1) % 4 == 2)
				return Vector2i (0, textureBox.height);
			else
				if ((typeOfObject - 1) % 4 == 3)
					return Vector2i (0, 0);

	return Vector2i (0, 0);
}

Vector2f GroundConnection::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int GroundConnection::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void GroundConnection::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement groundConnectionBody;
	groundConnectionBody.path = "Game/worldSprites/terrainObjects/ground/groundConnection" + std::to_string(typeOfObject) + ".png";
	groundConnectionBody.size = Vector2f(conditionalSizeUnits);
	groundConnectionBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(groundConnectionBody);
}