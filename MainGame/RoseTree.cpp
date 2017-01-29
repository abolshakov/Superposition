#include "RoseTree.h"

RoseTree::RoseTree(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	strength = 0;
}

Vector2i RoseTree::calculateTextureOffset()
{
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

void RoseTree::initPedestal()
{
	focus1 = Vector2i(textureBox.left + int(textureBox.width / 2.15), int(position.y));
	focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 2.15), int(position.y));
	ellipseSize = int((focus2.x - focus1.x) * 1.3);
}

std::string RoseTree::getSpriteName(long long elapsedTime)
{
	return "roseTree" + typeOfImage + ".png";
}