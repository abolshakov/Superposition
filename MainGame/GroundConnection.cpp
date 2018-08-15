#include "GroundConnection.h"
#include "Helper.h"

GroundConnection::GroundConnection(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 20;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "groundConnection";
	setType(typeOfObject);
}

void GroundConnection::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	if ((typeOfObject - 1) % 4 == 0)
	{
		this->conditionalSizeUnits = Vector2f(276, 1000);
	}
	else
		if ((typeOfObject - 1) % 4 == 1)
		{
			this->conditionalSizeUnits = Vector2f(276, 1000);
		}
		else
			if ((typeOfObject - 1) % 4 == 2)
			{
				this->conditionalSizeUnits = Vector2f(1000, 276);
			}
			else
				if ((typeOfObject - 1) % 4 == 3)
				{
					this->conditionalSizeUnits = Vector2f(1000, 276);
				}
	
	if (typeOfObject >= 1 && typeOfObject <= 4)
		this->zCoords = 1;
	else
		if (typeOfObject >= 5 && typeOfObject <= 8)
			this->zCoords = 2;
		else
			if (typeOfObject >= 9 && typeOfObject <= 12)
				this->zCoords = 3;
			else
				if (typeOfObject >= 13 && typeOfObject <= 16)
					this->zCoords = 4;
				else
					if (typeOfObject >= 17 && typeOfObject <= 20)
						this->zCoords = 5;
}

Vector2i GroundConnection::calculateTextureOffset()
{
	if ((typeOfObject - 1) % 4 == 0)
		return Vector2i(textureBox.width, 0);
	else
		if ((typeOfObject - 1) % 4 == 1)
			return Vector2i(0, 0);
		else
			if ((typeOfObject - 1) % 4 == 2)
				return Vector2i(0, textureBox.height);
			else
				if ((typeOfObject - 1) % 4 == 3)
					return Vector2i(0, 0);

	return Vector2i(0, 0);
}

std::string GroundConnection::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/ground/groundConnection" + std::to_string(typeOfObject) + ".png";
}