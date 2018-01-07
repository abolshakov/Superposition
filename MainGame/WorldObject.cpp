#include "WorldObject.h"
#include "Helper.h"

WorldObject::WorldObject(std::string objectName, Vector2f centerPosition)
{
	name = objectName;
	position = centerPosition;
	WorldObject::initPedestal();
	isVisibleInventory = false;
	//light = std::make_shared<ltbl::LightPointEmission>();
}

void WorldObject::setTextureSize(Vector2i textureSize)
{
	textureBox.width = textureSize.x;
	textureBox.height = textureSize.y;
	originalTextureBox = textureBox;
	textureBoxOffset = calculateTextureOffset();

	textureBox.left = int(position.x) - textureBoxOffset.x;
	textureBox.top = int(position.y) - textureBoxOffset.y;
	
	initPedestal();
}

Vector2f WorldObject::getBias(Vector2f focusedObjectPos, double elapsedTime)
{
	biasOffset.x = bias.x*(focusedObjectPos.y - position.y) / elapsedTime;
	//biasOffset.x = bias.x*(focusedObjectPos.y - position.y)/elapsedTime;
	biasOffset.y = bias.y*(focusedObjectPos.y - position.y)/elapsedTime;

	return Vector2f(biasOffset.x, biasOffset.y);
}

Vector2f WorldObject::getScaleRatio()
{
	return Vector2f(conditionalSizeUnits.x / originalTextureBox.width, conditionalSizeUnits.y / originalTextureBox.height);
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

