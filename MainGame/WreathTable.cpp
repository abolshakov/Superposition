#include "WreathTable.h"
#include "Helper.h"
#include <fstream>
#include "ObjectInitializer.h"

WreathTable::WreathTable(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 200;
	plateRadius = 100;
	toSaveName = "wreathTable";
	setType(typeOfObject);
	mirrored = false;
	initCraftRecipes();
	tag = Tag::wreathTable;
}

void WreathTable::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i(588, 523);
}

Vector2i WreathTable::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.3));
}

void WreathTable::initPedestal()
{
	focus1 = Vector2f(position.x - textureBox.width / 4, position.y);
	focus2 = Vector2f(position.x + textureBox.width / 4, position.y);
	ellipseSize = float((focus2.x - focus1.x) * 1.2);
}

void WreathTable::initCraftRecipes()
{
	std::ifstream fin(craftRecipesPath);

	int craftedItemId, recipeSize;

	while (fin >> craftedItemId >> recipeSize)
	{
		std::vector<std::pair<Tag, int>> recipe = {};
		for (int i = 0; i < recipeSize; i++)
		{
			int ingredientId = 0, ingredientCount = 0;
			fin >> ingredientId >> ingredientCount;
			recipe.push_back(std::make_pair(Tag(ingredientId), ingredientCount));
		}
		craftRecipes[Tag(craftedItemId)].push_back(recipe);
	}

	fin.close();
}

void WreathTable::resultAnalysis()
{
	if (craftResult == Tag::emptyCell)
		return;

	if (craftResult == Tag::heroBag)
	{

	}
}

Tag WreathTable::checkCraftResult()
{
	for (auto& recipes : craftRecipes)
		for (auto& recipe : recipes.second)
		{
			bool match = true;
			for (auto ingredient : recipe)
				if (currentCraft.count(ingredient.first) < ingredient.second)
				{
					match = false;
					continue;
				}
			if (match)
			{
				buildSystem->selectedObject = recipes.first;
				return recipes.first;
			}
		}

	return Tag::emptyCell;
}

void WreathTable::putItemToCraft(Tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f WreathTable::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int WreathTable::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void WreathTable::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement body;
	body.path = "Game/worldSprites/terrainObjects/wreathTable/wreathTable.png";
	body.size = Vector2f(conditionalSizeUnits);
	body.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(body);
}