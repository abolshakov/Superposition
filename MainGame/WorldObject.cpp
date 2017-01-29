#include "WorldObject.h"

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	WorldObject::initPedestal();
}

void WorldObject::setTextureSize(Vector2i textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;

	textureBoxOffset = calculateTextureOffset();

	textureBox.left = int(position.x) - textureBoxOffset.x;
	textureBox.top = int(position.y) - textureBoxOffset.y;

	initPedestal();	
}

Vector2i WorldObject::calculateTextureOffset()
{
	return Vector2i();
}

void WorldObject::initPedestal()
{
}

void WorldObject::setPosition(Vector2f newPosition)
{
	position = newPosition;
	textureBox.left = int(newPosition.x) - textureBoxOffset.x;
	textureBox.top = int(newPosition.y) - textureBoxOffset.y;
}

WorldObject::~WorldObject()
{
}

