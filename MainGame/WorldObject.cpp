#include "WorldObject.h"

WorldObject::WorldObject(std::string objectName, FloatRect boundingBox, IntRect textureBoundingBox)
{
	name = objectName;
	box = boundingBox;
	textureBox = textureBoundingBox;
	size = Vector2i(box.width, box.height);
	texturePosition = Vector2i(textureBox.left, textureBox.top);
	textureSize = Vector2i(textureBox.width, textureBox.height);	
}

WorldObject::~WorldObject()
{
}

