/*#include "TreeOfGreatness.h"
#include "Helper.h"

TreeOfGreatness::TreeOfGreatness(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2f(800, 1050);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2f(800, 1050);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2f(1000, 1500);
	strength = 0;
	intersectsRadius = 200;
}

Vector2i TreeOfGreatness::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;

	if (typeOfObject == 1)
		return Vector2i(textureBox.width / 3, int(textureBox.height / 1.2));
	if (typeOfObject == 2)
		return Vector2i(textureBox.width / 1.7, int(textureBox.height / 1.25));
	if (typeOfObject == 3)
		return Vector2i(textureBox.width / 2.6, int(textureBox.height / 1.15));

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

void TreeOfGreatness::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 12, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 12, position.y);

		ellipseSize = int((focus2.x - focus1.x) * 1.4);		
	}
	if (typeOfObject == 2)
	{
		focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
	if (typeOfObject == 3)
	{
		focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 10, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.3);
	}
	//textureBoxOffset = Vector2i((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

std::string TreeOfGreatness::getSpriteName(long long elapsedTime)
{
	return "treeOfGreatness	" + std::to_string(typeOfObject) + ".png";
}*/