#include "WorldObject.h"
#include "Helper.h"

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	WorldObject::initPedestal();
	inventoryVisibility = false;
	randomSizeIncrease = Vector2i (0, 0);
}

void WorldObject::setTextureSize(Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;
	originalTextureBox = textureBox;
	textureBoxOffset = calculateTextureOffset();

	textureBox.left = int(position.x) - textureBoxOffset.x;
	textureBox.top = int(position.y) - textureBoxOffset.y;
	
	initPedestal();
}

Vector2f WorldObject::getScaleRatio()
{
	return Vector2f (float(conditionalSizeUnits.x) / originalTextureBox.width, float(conditionalSizeUnits.y) / originalTextureBox.height);
}

Vector2i WorldObject::calculateTextureOffset()
{
	return Vector2i(0, 0);
}

void WorldObject::initPedestal()
{
}

void WorldObject::setPosition(Vector2f newPosition)
{
	posOffset = Vector2f(position.x - previousPosition.x, position.y - previousPosition.y);
	previousPosition = position;
	position = Vector2f(newPosition);
	textureBox.left = int(newPosition.x) - textureBoxOffset.x;
	textureBox.top = int(newPosition.y) - textureBoxOffset.y;
}

WorldObject::~WorldObject()
{

}

