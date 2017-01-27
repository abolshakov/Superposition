#include "WorldObject.h"

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	initPedestal();
}

void WorldObject::setTextureSize(Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;

	textureBoxOffset = calculateTextureOffset();

	textureBox.left = position.x - textureBoxOffset.x;
	textureBox.top = position.y - textureBoxOffset.y;

	initPedestal();	
}

Vector2f WorldObject::calculateTextureOffset()
{
	return Vector2f();
}

void WorldObject::initPedestal()
{
}

void WorldObject::setPosition(Vector2f newPosition)
{
	position = newPosition;
	textureBox.left = newPosition.x - textureBoxOffset.x;
	textureBox.top = newPosition.y - textureBoxOffset.y;
}

WorldObject::~WorldObject()
{
}

