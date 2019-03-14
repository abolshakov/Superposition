#include "ForestTree.h"
#include "Helper.h"

ForestTree::ForestTree(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 12;
	this->typeOfObject = typeOfObject;
	strength = 0;
	radius = 50;
	animationSpeed = 0.0005f;
	toSaveName = "ForestTree";
	setType(typeOfObject);
	tag = forestTreeTag;
}

void ForestTree::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	if (typeOfObject == 1)
		conditionalSizeUnits = Vector2i (481, 738);
	if (typeOfObject == 2)
		conditionalSizeUnits = Vector2i (578, 763);
	if (typeOfObject == 3)
		conditionalSizeUnits = Vector2i (606, 926);
	if (typeOfObject == 4)
		conditionalSizeUnits = Vector2i (581, 927);
	if (typeOfObject == 5)
		conditionalSizeUnits = Vector2i (344, 904);
	if (typeOfObject == 6)
		conditionalSizeUnits = Vector2i (379, 745);
	if (typeOfObject == 7)
		conditionalSizeUnits = Vector2i (418, 1102);
	if (typeOfObject == 8)
		conditionalSizeUnits = Vector2i (415, 1005);
	if (typeOfObject == 9)
		conditionalSizeUnits = Vector2i (328, 851);
	if (typeOfObject == 10)
		conditionalSizeUnits = Vector2i(693, 1004);
	if (typeOfObject == 11)
		conditionalSizeUnits = Vector2i(673, 1032);
	if (typeOfObject == 12)
		conditionalSizeUnits = Vector2i(1054, 961);
}

Vector2i ForestTree::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);

	if (typeOfObject == 1)
		return Vector2i (textureBox.width / 2.2, int(textureBox.height / 1.1));
	if (typeOfObject == 2)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 3)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 4)
		return Vector2i (textureBox.width / 1.8, int(textureBox.height / 1.05));
	if (typeOfObject == 5)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 6)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 7)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 8)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 9)
		return Vector2i (textureBox.width / 2, int(textureBox.height / 1.05));
	if (typeOfObject == 10)
		return Vector2i(textureBox.width / 2.4, int(textureBox.height / 1.1));
	if (typeOfObject == 11)
		return Vector2i(textureBox.width / 1.8, int(textureBox.height / 1.1));
	if (typeOfObject == 12)
		return Vector2i(textureBox.width / 2.4, int(textureBox.height / 1.1));

	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.25));
}

void ForestTree::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f (position.x - textureBox.width / 10, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 10, position.y);

		ellipseSize = float((focus2.x - focus1.x) * 1.4);
	}
	else
		if (typeOfObject == 2)
		{
			focus1 = Vector2f (position.x - textureBox.width / 10, position.y);
			focus2 = Vector2f (position.x + textureBox.width / 10, position.y);
			ellipseSize = float((focus2.x - focus1.x) * 1.4);
		}
		else
			if (typeOfObject == 3)
			{
				focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
				focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
				ellipseSize = float((focus2.x - focus1.x) * 1.4);
			}
			else
				if (typeOfObject == 4)
				{
					focus1 = Vector2f (position.x - textureBox.width / 6, position.y);
					focus2 = Vector2f (position.x + textureBox.width / 6, position.y);

					ellipseSize = float((focus2.x - focus1.x) * 1.4);
				}
				else
					if (typeOfObject == 5)
					{
						focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
						focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
						ellipseSize = float((focus2.x - focus1.x) * 1.4);
					}
					else
						if (typeOfObject == 6)
						{
							focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
							focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
							ellipseSize = float((focus2.x - focus1.x) * 1.4);
						}
						else
							if (typeOfObject == 7)
							{
								focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
								focus2 = Vector2f (position.x + textureBox.width / 8, position.y);

								ellipseSize = float((focus2.x - focus1.x) * 1.4);
							}
							else
								if (typeOfObject == 8)
								{
									focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
									focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
									ellipseSize = float((focus2.x - focus1.x) * 1.4);
								}
								else
									if (typeOfObject == 9)
									{
										focus1 = Vector2f (position.x - textureBox.width / 8, position.y);
										focus2 = Vector2f (position.x + textureBox.width / 8, position.y);
										ellipseSize = float((focus2.x - focus1.x) * 1.4);
									}
									else
										if (typeOfObject == 10)
										{
											focus1 = Vector2f(position.x - textureBox.width / 8, position.y);
											focus2 = Vector2f(position.x + textureBox.width / 8, position.y);

											ellipseSize = float((focus2.x - focus1.x) * 1.4);
										}
										else
											if (typeOfObject == 11)
											{
												focus1 = Vector2f(position.x - textureBox.width / 8, position.y);
												focus2 = Vector2f(position.x + textureBox.width / 8, position.y);
												ellipseSize = float((focus2.x - focus1.x) * 1.4);
											}
											else
												if (typeOfObject == 12)
												{
													focus1 = Vector2f(position.x - textureBox.width / 8, position.y);
													focus2 = Vector2f(position.x + textureBox.width / 8, position.y);
													ellipseSize = float((focus2.x - focus1.x) * 1.2);
												}
	//textureBoxOffset = Vector2f ((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

Vector2f ForestTree::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int ForestTree::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void ForestTree::prepareSpriteNames(long long elapsedTime)
{
    additionalSprites.clear();
    spriteChainElement treeBody;
	treeBody.size = Vector2f(conditionalSizeUnits);
	treeBody.offset = Vector2f(textureBoxOffset);

	switch (state)
	{
		case common:
		{
            animationLength = 1;
            treeBody.path = "Game/worldSprites/terrainObjects/forestTree/forestTree" + std::to_string(typeOfObject) + ".png";
            break;
		}
		case absorbed:
		{
			animationLength = 15;
			treeBody.path = "Game/worldSprites/terrainObjects/forestTree/forestTree" + std::to_string(typeOfObject) + ".png";
			transparensy = 100 - currentSprite * 100 / animationLength;
			break;
		}
	}

    additionalSprites.push_back(treeBody);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			if (state == absorbed)
			{
				state = common;
				delatePromiseOn();
			}
			currentSprite = 1;
		}
	}
}