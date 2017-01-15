#include "WorldObject.h"

WorldObject::WorldObject(std::string objectName, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize)
{
	name = objectName;
	box = boundingBox;
	textureBoxOffset = textureOffset;
	size = Vector2i(box.width, box.height);
	textureBox = IntRect(box.left - textureBoxOffset.x, box.top - textureBoxOffset.y, textureBoxSize.x, textureBoxSize.y);	
}

WorldObject::~WorldObject()
{
}

