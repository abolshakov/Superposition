#include "Chamomile.h"
#include "Helper.h"

Chamomile::Chamomile(std::string objectName, Vector2f centerPosition, int typeOfObject) : PickedObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	toSaveName = "chamomile";
	setType(typeOfObject);
	tag = chamomileTag;

	count = 1;
	id = 3;	
	maxCount = 4;
}

void Chamomile::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(200, 200);
}

Vector2i Chamomile::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void Chamomile::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 10, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.08);
	}
}

std::string Chamomile::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/chamomile/chamomile" + std::to_string(typeOfObject) + ".png";
}