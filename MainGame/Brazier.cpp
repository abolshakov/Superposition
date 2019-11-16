#include "Brazier.h"
#include "Helper.h"
#include <fstream>
#include "ObjectInitializer.h"

Brazier::Brazier(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 450;
	plateRadius = 100;
	toSaveName = "brazier";
	setType(typeOfObject);
	isMultiellipse = true;
	mirrored = false;
	initCraftRecipes();
	tag = Tag::brazier;
}

void Brazier::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (900, 900);
}

Vector2i Brazier::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.4));
}

void Brazier::initPedestal()
{
	if (typeOfObject == 1)
	{
		/*focus1 = Vector2f (position.x - textureBox.width / 4, position.y);
		focus2 = Vector2f (position.x + textureBox.width / 4, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.17);*/
		focus1 = Vector2f(position.x, position.y);
		focus2 = Vector2f(position.x, position.y);
		ellipseSize = 0;

		std::pair<std::pair<float, Vector2f>, std::pair<Vector2f, Vector2f>> microEllipse;
		microEllipse.second.first = Vector2f(position.x - textureBox.width / 3, position.y + textureBox.height / 25);
		microEllipse.second.second = Vector2f(position.x - textureBox.width / 8, position.y + textureBox.height / 25);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.2;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);

		microEllipse.second.first = Vector2f(position.x - textureBox.width / 4, position.y - textureBox.height / 25);
		microEllipse.second.second = Vector2f(position.x, position.y - textureBox.width / 25);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.2;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);

		microEllipse.second.first = Vector2f(position.x, position.y - textureBox.width / 50);
		microEllipse.second.second = Vector2f(position.x + textureBox.width / 4, position.y - textureBox.height / 50);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.17;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);

		microEllipse.second.first = Vector2f(position.x + textureBox.width / 8, position.y + textureBox.height / 25);
		microEllipse.second.second = Vector2f(position.x + textureBox.width / 2.5, position.y + textureBox.height / 25);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.17;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);

		microEllipse.second.first = Vector2f(position.x - textureBox.width / 16, position.y + textureBox.height / 6);
		microEllipse.second.second = Vector2f(position.x + textureBox.width / 16, position.y + textureBox.height / 6);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.17;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);

		microEllipse.second.first = Vector2f(position.x + textureBox.width / 5.5, position.y + textureBox.height / 6.5);
		microEllipse.second.second = Vector2f(position.x + textureBox.width / 3.9, position.y + textureBox.height / 6.5);
		microEllipse.first.first = (microEllipse.second.second.x - microEllipse.second.first.x) * 1.95;
		microEllipse.first.second = Vector2f((microEllipse.second.first.x + microEllipse.second.second.x) / 2, (microEllipse.second.first.y + microEllipse.second.second.y) / 2);
		internalEllipses.push_back(microEllipse);
	}
}

void Brazier::initCraftRecipes()
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

void Brazier::resultAnalysis()
{
	if (craftResult == Tag::emptyCell)
		return;

	if (craftResult == Tag::heroBag)
	{
		
	}
}

Tag Brazier::checkCraftResult()
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

void Brazier::putItemToCraft(Tag id)
{
	if (currentCraft.count(id) > 0)
		currentCraft.at(id)++;
	else
		currentCraft[id]++;

	craftResult = checkCraftResult();
}

Vector2f Brazier::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Brazier::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Brazier::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	additionalSprites.clear();
	spriteChainElement brazierBack, brazierFront, fire, craftIcon;
	brazierBack.path = "Game/worldSprites/terrainObjects/brazier/brazier2.png";
	brazierBack.size = Vector2f(conditionalSizeUnits);	
	brazierBack.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y - conditionalSizeUnits.y * 0.2f);
	brazierBack.position = { position.x, position.y - conditionalSizeUnits.y * 0.2f };
	if (!currentCraft.empty())
	{
		fire.path = "Game/worldSprites/terrainObjects/brazier/fire.png";
		fire.size = Vector2f(conditionalSizeUnits.x * 0.22f, conditionalSizeUnits.y * 0.22f);
		fire.offset = Vector2f(conditionalSizeUnits.x * 0.06f, textureBoxOffset.y - conditionalSizeUnits.y * 0.555f + scaleFactor * conditionalSizeUnits.y / 20);
		fire.position = { position.x, position.y };
		fire.antiTransparent = true;
	}
    brazierFront.path = "Game/worldSprites/terrainObjects/brazier/brazier1.png";
	brazierFront.size = Vector2f(conditionalSizeUnits);
	brazierFront.offset = Vector2f(textureBoxOffset.x, textureBoxOffset.y + conditionalSizeUnits.y * 0.2f);
	brazierFront.position = { position.x, position.y + conditionalSizeUnits.y * 0.2f };
	brazierFront.antiTransparent = true;
	additionalSprites.push_back(brazierBack);
	additionalSprites.push_back(fire);
    additionalSprites.push_back(brazierFront);
}