#include "BuildSystemMaker.h"

BuildSystemMaker::BuildSystemMaker()
{

}

BuildSystemMaker::~BuildSystemMaker()
{

}

void BuildSystemMaker::Init(std::unordered_map<int, cell> itemsSpriteList)
{
	this->craftIngredientsSpriteList = itemsSpriteList;
	inicializeObjectsInfo();
	initializeButtons();

	font.loadFromFile("Fonts/Bebas.ttf");
	numberOfObjects.setFont(font);
	numberOfObjects.setCharacterSize(30);
	numberOfObjects.setFillColor(Color(200, 200, 200, 255));
}

void BuildSystemMaker::initializeButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

	//initialize buildStart button
	buildStartButtonTexture.loadFromFile("World/buildSystem/buildStart.png");
	buildStartButton.setTexture(buildStartButtonTexture);
	buildStartButton.setPosition(buildStartButton.getTextureRect().width / 8, 10);
	buildStartButton.setScale(screenSize.y / 15 / buildStartButtonTexture.getSize().x, screenSize.y / 15 / buildStartButtonTexture.getSize().y);

	//initialize buildStop button
	buildStopButtonTexture.loadFromFile("World/buildSystem/buildStop.png");
	buildStopButton.setTexture(buildStopButtonTexture);
	buildStopButton.setPosition(buildStopButton.getTextureRect().width / 8, 10);
	buildStopButton.setScale(buildStartButton.getScale());

	//initialize icons of built objects 
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconTexture.loadFromFile(builtObjects[i].icon);
		builtObjects[i].iconSprite.setTexture(builtObjects[i].iconTexture);		
		builtObjects[i].iconSprite.setScale(buildStartButton.getScale());
		builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getGlobalBounds().height * 1.25);
	}

	//initialize frame for recipes
	recipeFrame.setScale((float)builtObjects[0].iconTexture.getSize().x / recipeFrame.getTextureRect().width * 3,
		(float)builtObjects[0].iconTexture.getSize().y / recipeFrame.getTextureRect().height * 2);
}

void BuildSystemMaker::inventoryBounding(std::vector<std::pair <int, int>>& inventory)
{
	currentInventory = std::ref(inventory);
}

void BuildSystemMaker::inicializeObjectsInfo()
{
	//recipeFrameTexture.loadFromFile("World/BuildSystemMaker/recipeFrame.png");
	//recipeFrame.setTexture(recipeFrameTexture);

	std::string objectIconPath, objectImagePath, objectImageType;
	std::ifstream fin(BuildSystemMakerObjectsInfoFileDirectory);
	while (fin >> objectIconPath >> objectImagePath >> objectImageType)
	{
		objectInfo infoItem;

		infoItem.icon = objectIconPath;
		infoItem.image = objectImagePath;
		infoItem.type = objectImageType;

		int recipeItemId, recipeItemAmount, recipeLength;

		fin >> recipeLength;
		for (int i = 0; i < recipeLength; i++)
		{
			fin >> recipeItemId >> recipeItemAmount;
			infoItem.recipe.push_back(std::make_pair(recipeItemId, recipeItemAmount));
		}

		builtObjects.push_back(infoItem);
	}
}

void BuildSystemMaker::draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f focusedObjectPosition, std::vector<WorldObject*> visibleItems)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (!isBuilding)
	{
		window.draw(buildStartButton);
		return;
	}
	
	animator(elapsedTime);

	if (selectedObject != -1)
	{
		//initialize spriteSilhouette
		auto sprite = (&spriteMap[builtObjects[selectedObject].image])->sprite;
		auto terrain = dynamic_cast<TerrainObject*>(staticGrid.getItemByName(builtObjects[selectedObject].type));

		sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
		sprite.setScale(terrain->getScaleRatio().x*scaleFactor, terrain->getScaleRatio().y*scaleFactor*sqrt(sqrt(scaleFactor)));
		Vector2f mouseWorldPos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x*scaleFactor) / scaleFactor,
			(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);

		//enumeration of intersecting elements
		canBePlaced = true;
		for (auto item : visibleItems)
		{
			if (item->isBackground)
				continue;
			auto currentTerrain = dynamic_cast<TerrainObject*>(item);
			if (currentTerrain)
				if (Helper::isIntersectTerrain(mouseWorldPos, *currentTerrain, terrain->intersectsRadius))
				{
					sprite.setColor(Color::Red);
					canBePlaced = false;
					break;
				}
		}

		if (canBePlaced)
			sprite.setColor(Color::Green);
		Vector2f testPosition = Vector2f(mousePos.x - terrain->getTextureOffset().x*scaleFactor, mousePos.y + (terrain->getTextureSize().y - terrain->getTextureOffset().y)*scaleFactor);
		sprite.setPosition(Vector2f(mousePos.x - terrain->getTextureOffset().x*scaleFactor, mousePos.y + (terrain->getTextureSize().y - terrain->getTextureOffset().y)*scaleFactor));

		window.draw(sprite);
	}

	window.draw(buildStopButton);

	//draw icons
	for (int i = 0; i < builtObjects.size(); i++)
	{
		window.draw(builtObjects[i].iconSprite);
	}

	if (isRecipeFrame)
	{
		//window.draw(recipeFrame);
		for (int i = 0; i < builtObjects[currentObject].recipe.size(); i++)
		{
			Sprite *currentRecipeItem = &craftIngredientsSpriteList[builtObjects[currentObject].recipe[i].first].sprite;
			currentRecipeItem->setPosition(recipeFrame.getPosition().x + i * currentRecipeItem->getTextureRect().width, recipeFrame.getPosition().y);
			window.draw(*currentRecipeItem);

			int boundInventoryObjectsCount = 0;
			for (auto item = currentInventory.get().begin(); item != currentInventory.get().end(); ++item)
			{
				if (item->first == builtObjects[currentObject].recipe[i].first)
					boundInventoryObjectsCount+= item->second;
			}

			numberOfObjects.setString(std::to_string(boundInventoryObjectsCount) + '/' + std::to_string(builtObjects[currentObject].recipe[i].second));
			if (boundInventoryObjectsCount < builtObjects[currentObject].recipe[i].second)
				numberOfObjects.setFillColor(Color(193, 63, 63, 255));
			numberOfObjects.setPosition(currentRecipeItem->getPosition().x + currentRecipeItem->getTextureRect().width - numberOfObjects.getGlobalBounds().width,
				currentRecipeItem->getPosition().y + currentRecipeItem->getTextureRect().height - numberOfObjects.getGlobalBounds().height);
			window.draw(numberOfObjects);
			numberOfObjects.setFillColor(Color(200, 200, 200, 255));
		}
		 
	}
}

void BuildSystemMaker::interact()
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (isBuilding)
	{
		bool f = false;
		for (int i = 0; i < builtObjects.size(); i++)
		{
			if (Helper::isIntersects(mousePos, IntRect(builtObjects[i].iconSprite.getPosition().x, builtObjects[i].iconSprite.getPosition().y, builtObjects[i].iconSprite.getTextureRect().width, builtObjects[i].iconSprite.getTextureRect().height)))
			{
				currentObject = i;
				isRecipeFrame = true;
				recipeFrame.setPosition(builtObjects[i].iconSprite.getPosition().x + builtObjects[i].iconSprite.getTextureRect().width*1.25, builtObjects[i].iconSprite.getPosition().y);
				f = true;
				break;
			}
		}
		if (!f)
		{
			isRecipeFrame = false;
			currentObject = -1;
		}
	}
}

void BuildSystemMaker::onMouseDownInteract(Vector2f focusedObjectPosition, float scaleFactor)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();

	if (Helper::isIntersects(mousePos, buildStartButton.getTextureRect()) && !isBuilding)
	{
		isBuilding = true;
		currentObject = -1;
	}
	else
		if (Helper::isIntersects(mousePos, buildStopButton.getTextureRect()) && isBuilding)
		{
			isBuilding = false;
			animationSpeed = 0.001;
			for (int i = 0; i < builtObjects.size(); i++)
			{
				builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getGlobalBounds().height * 1.25);
			}
			return;
		}

	if (isBuilding && currentObject != -1 && canAfford())
	{
		selectedObject = currentObject;
		return;
	}

	if (isBuilding && selectedObject != -1 && currentObject == -1 && canBePlaced)
		buildingPosition = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x * scaleFactor) / scaleFactor,
			(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);
	else
		buildingPosition = Vector2f(-1, -1);
}

bool BuildSystemMaker::canAfford()
{
	if (currentObject != -1)
	{
		std::vector<std::pair <int, int>> temporaryInventory = builtObjects[currentObject].recipe;
		std::vector<std::pair <int, int>> curInventory = currentInventory.get();

		for (auto curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
		{
			for (auto curInvItem = curInventory.begin(); curInvItem != curInventory.end(); ++curInvItem)
			{
				if (curRecipeItem->first == curInvItem->first)
				{
					if (curInvItem->second >= curRecipeItem->second)
					{
						curRecipeItem->second = 0;
						break;
					}
					else
						curRecipeItem->second -= curInvItem->second;
				}
			}
			if (curRecipeItem->second != 0)
			{
				return false;
			}
		}

		return true;
	}
	return false;
}

void BuildSystemMaker::wasPlaced()
{
	std::vector<std::pair <int, int>> temporaryInventory = builtObjects[selectedObject].recipe;

	for (auto curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
	{
		for (auto curInvItem = currentInventory.get().begin(); curInvItem != currentInventory.get().end(); ++curInvItem)
		{
			if (curRecipeItem->first == curInvItem->first)
			{
				if (curInvItem->second >= curRecipeItem->second)
				{					
					curInvItem->second -= curRecipeItem->second;
					if (curInvItem->second == 0)
						curInvItem->first = 0;
					curRecipeItem->second = 0;
				}
				else
				{
					curRecipeItem->second -= curInvItem->second;
					curInvItem->second = 0;
					curInvItem->first = 0;
				}
			}
		}
	}

	selectedObject = -1;
	buildingPosition = Vector2f(-1, -1);
}

void BuildSystemMaker::animator(float elapsedTime)
{
	if (builtObjects[0].iconSprite.getPosition().x < builtObjects[0].iconSprite.getTextureRect().width / 8 && animationSpeed > 0)
		animationSpeed -= (float)elapsedTime / 400000000;
	else
	{
		animationSpeed = 0;
		for (int i = 0; i < builtObjects.size(); i++)
		{
			builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getTextureRect().width / 8 + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
		}
		exit;
	}
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getPosition().x + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
	}
}

int BuildSystemMaker::getBuiltObjectType()
{
	if (selectedObject == -1)
		return -1;

	std::string objectType = builtObjects[selectedObject].type;
	if (objectType == "bonefireOfInsight")
		return 4;
	if (objectType == "homeCosiness")
		return 5;
}
