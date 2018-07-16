#include "TreeOfGreatness.h"
#include "Helper.h"

TreeOfGreatness::TreeOfGreatness(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 9;
	this->typeOfObject = typeOfObject;
	strength = 0;
	intersectsRadius = 200;
	toSaveName = "treeOfGreatness";
	setType(typeOfObject);
}

void TreeOfGreatness::setType(int typeOfObject)
{
	/*this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2f(150, 150);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2f(400, 400);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2f(150, 150);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2f(150, 150);*/

	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2f(500, 800);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2f(500, 800);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2f(500, 800);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2f(300, 800);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2f(500, 800);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2f(350, 800);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2f(400, 800);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2f(350, 800);
}

Vector2i TreeOfGreatness::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;

	if (typeOfObject == 1)
		return Vector2i(textureBox.width / 2.2, int(textureBox.height / 1.1));
	if (typeOfObject == 2)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 3)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 4)
		return Vector2i(textureBox.width / 1.8, int(textureBox.height / 1.05));
	if (typeOfObject == 5)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 6)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 7)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 8)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 9)
		return Vector2i(textureBox.width / 2, int(textureBox.height / 1.05));

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

void TreeOfGreatness::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 10, position.y);

		ellipseSize = int((focus2.x - focus1.x) * 1.4);
	}
	else
		if (typeOfObject == 2)
		{
			focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
			focus2 = Vector2i(position.x + textureBox.width / 10, position.y);
			ellipseSize = int((focus2.x - focus1.x) * 1.4);
		}
		else
			if (typeOfObject == 3)
			{
				focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
				focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
				ellipseSize = int((focus2.x - focus1.x) * 1.4);
			}
			else
				if (typeOfObject == 4)
				{
					focus1 = Vector2i(position.x - textureBox.width / 6, position.y);
					focus2 = Vector2i(position.x + textureBox.width / 6, position.y);

					ellipseSize = int((focus2.x - focus1.x) * 1.4);
				}
				else
					if (typeOfObject == 5)
					{
						focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
						focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
						ellipseSize = int((focus2.x - focus1.x) * 1.4);
					}
					else
						if (typeOfObject == 6)
						{
							focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
							focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
							ellipseSize = int((focus2.x - focus1.x) * 1.4);
						}
						else
							if (typeOfObject == 7)
							{
								focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
								focus2 = Vector2i(position.x + textureBox.width / 8, position.y);

								ellipseSize = int((focus2.x - focus1.x) * 1.4);
							}
							else
								if (typeOfObject == 8)
								{
									focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
									focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
									ellipseSize = int((focus2.x - focus1.x) * 1.4);
								}
								else
									if (typeOfObject == 9)
									{
										focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
										focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
										ellipseSize = int((focus2.x - focus1.x) * 1.4);
									}
	//textureBoxOffset = Vector2i((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

std::string TreeOfGreatness::getSpriteName(long long elapsedTime)
{
	return "terrainObjects/treeOfGreatness/treeOfGreatness" + std::to_string(typeOfObject) + ".png";
}