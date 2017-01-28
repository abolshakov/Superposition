#include "RoseTree.h"

RoseTree::RoseTree(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
}

Vector2f RoseTree::calculateTextureOffset()
{
	return Vector2f(textureBox.width / 2, textureBox.height / 1.25);
}

void RoseTree::initPedestal()
{
	focus1 = Vector2f(textureBox.left + textureBox.width / 2.15, position.y);
	focus2 = Vector2f(textureBox.left + textureBox.width - textureBox.width / 2.15, position.y);
	ellipseSize = (focus2.x-focus1.x)*1.3;
}

const std::string RoseTree::getSpriteName(long long elapsedTime)
{
	return "roseTree" + typeOfImage + ".png";
}