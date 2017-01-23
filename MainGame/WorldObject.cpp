#include "WorldObject.h"

WorldObject::WorldObject(std::string objectName, Vector2f ellipseCenterPosition)
{
	name = objectName;
	position = ellipseCenterPosition;
	initEllipse();
}

void WorldObject::setTextureSize(Vector2f textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;

	textureBoxOffset = calculateTextureOffset();

	textureBox.left = position.x - textureBoxOffset.x;
	textureBox.top = position.y - textureBoxOffset.y;

	initEllipse();	
}

Vector2f WorldObject::calculateTextureOffset()
{
	return Vector2f();
}

void WorldObject::initEllipse()
{
}

WorldObject::~WorldObject()
{
}

