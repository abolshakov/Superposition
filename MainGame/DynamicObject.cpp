#include "DynamicObject.h"

DynamicObject::DynamicObject(std::string objectName, Vector2f ellipseCenterPosition) : WorldObject(objectName, ellipseCenterPosition)
{
	speed = 0;
	direction = STAND;
	this->ellipseCenterPosition = ellipseCenterPosition;	
}

void DynamicObject::setPosition(Vector2f newPosition)
{
	ellipseCenterPosition = newPosition;
	textureBox.left = newPosition.x - textureBoxOffset.x;
	textureBox.top = newPosition.y - textureBoxOffset.y;
}

void DynamicObject::handleInput()
{
}




