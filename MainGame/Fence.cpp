#include "Fence.h"
#include "Helper.h"

Fence::Fence(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 3;
	this->typeOfObject = typeOfObject;
	radius = 120;
	toSaveName = "fence";
	setType(typeOfObject);
	isDotsAdjusted = true;
	tag = Tag::fence;
}

void Fence::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	switch (typeOfObject)
	{
		case 1:
		{
			this->conditionalSizeUnits = Vector2i (240, 230);
			break;
		}
		case 2:
		{
			this->conditionalSizeUnits = Vector2i (240, 230);
			break;
		}
		case 3:
		{
			this->conditionalSizeUnits = Vector2i (95, 350);
			break;
		}
		case 4:
		{
			this->conditionalSizeUnits = Vector2i (95, 350);
			break;
		}
		default:
		{
			this->conditionalSizeUnits = Vector2i (300, 300);
			break;
		}
	}
}

Vector2i Fence::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	switch (typeOfObject)
	{
		case 1:
		{
			return Vector2i(textureBox.width / 2, int(textureBox.height / 1));
			break;
		}
		case 2:
		{
			return Vector2i(textureBox.width / 2, int(textureBox.height / 1));
			break;
		}
		case 3:
		{
			return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
			break;
		}
		case 4:
		{
			return Vector2i(textureBox.width / 2, int(textureBox.height / 1));
			break;
		}
		default:
		{
			return Vector2i(textureBox.width / 2, int(textureBox.height / 1));
			break;
		}
	}
}

void Fence::initPedestal()
{
	switch (typeOfObject)
	{
		case 1:
		{
			dot1 = Vector2f (position.x - textureBox.width / 3.2, position.y);
			dot2 = Vector2f (position.x + textureBox.width / 1.8, position.y);
			break;
		}
		case 2:
		{
			dot1 = Vector2f(position.x - textureBox.width / 3.2, position.y);
			dot2 = Vector2f(position.x + textureBox.width / 1.8, position.y);
			break;
		}
		case 3:
		{
			dot1 = Vector2f(position.x, position.y - textureBox.height / 9);
			dot2 = Vector2f(position.x, position.y + textureBox.height / 2.8);
			break;
		}
		case 4:
		{
			dot1 = Vector2f(position.x - textureBox.width / 4, position.y/* - textureBox.height / 7*/);
			dot2 = Vector2f(position.x - textureBox.width / 4, position.y + textureBox.height / 2);
			break;
		}
		default:
		{
			dot1 = Vector2f(position.x - textureBox.width / 2, position.y);
			dot2 = Vector2f(position.x + textureBox.width / 2, position.y);
			break;
		}
	}

	//radius = sqrt(pow(dot1.x - dot2.x, 2) + pow(dot1.y + dot2.y, 2)) / 2 - 10;
}

Vector2f Fence::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	const Vector2f mousePos = Vector2f (Mouse::getPosition());
	const Vector2f mouseWorldPos = Vector2f ((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y * scaleFactor) / scaleFactor);

	const Vector2f dot1 = Vector2f ((this->dot1.x - this->position.x) + mouseWorldPos.x, (this->dot1.y - this->position.y) + mouseWorldPos.y);
	const Vector2f dot2 = Vector2f ((this->dot2.x - this->position.x) + mouseWorldPos.x, (this->dot2.y - this->position.y) + mouseWorldPos.y);

	for (auto&item : visibleItems)
	{
		if (item->tag == Tag::fence)
		{
			auto object = dynamic_cast<Fence*>(item);

			float const dist1 = sqrt(pow(dot1.x - object->getDot2().x, 2) + pow(dot1.y - object->getDot2().y, 2));
			float const dist2 = sqrt(pow(dot2.x - object->getDot1().x, 2) + pow(dot2.y - object->getDot1().y, 2));
			float const dist3 = sqrt(pow(dot1.x - object->getDot1().x, 2) + pow(dot1.y - object->getDot1().y, 2));
			float const dist4 = sqrt(pow(dot2.x - object->getDot2().x, 2) + pow(dot2.y - object->getDot2().y, 2));

			Vector2f ownDot = Vector2f (-1, -1), objDot = Vector2f (-1, -1);

			if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4 && dist1 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot1.x, this->position.y - this->dot1.y);
				objDot = Vector2f (object->getDot2());				
			}
			else
			if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4 && dist2 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot2.x, this->position.y - this->dot2.y);
				objDot = Vector2f (object->getDot1());				
			}
			else
			if (dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4 && dist3 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot1.x, this->position.y - this->dot1.y);
				objDot = Vector2f (object->getDot1());				
			}
			else
			if (dist4 <= dist1 && dist4 <= dist2 && dist4 <= dist3 && dist4 < 100)
			{
				ownDot = Vector2f (this->position.x - this->dot2.x, this->position.y - this->dot2.y);
				objDot = Vector2f (object->getDot2());				
			}			

			if (ownDot != Vector2f (-1, -1) && objDot != Vector2f (-1, -1))
			{			
				currentDot = Vector2f (objDot);
				return Vector2f (objDot.x + ownDot.x, objDot.y + ownDot.y);
			}
		}
	}
	return { -1, -1 };
}

int Fence::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	if (otherPos != Vector2f (-1, -1))
	{
		auto side = Helper::getSide(ounPos, otherPos);
		if (side == up)
			return 3;
		if (side == down)
			return 3;
		if (side == left)
			return 1;
		if (side == right)
			return 2;
	}

	return 1;
}

void Fence::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement fullSprite;
	fullSprite.path = "Game/worldSprites/terrainObjects/fence/fence" + std::to_string(typeOfObject) + ".png";
	fullSprite.size = Vector2f(conditionalSizeUnits);
	fullSprite.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(fullSprite);
}