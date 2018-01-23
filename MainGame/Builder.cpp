#include "Builder.h"

Builder::Builder()
{
}

void Builder::Init(std::unordered_map<int, cell> itemsSpriteList)
{
	this->craftIngredientsSpriteList = itemsSpriteList;
	inicializeObjectsInfo();
	initializeButtons();
}

void Builder::initializeButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

	//initialize buildStart button
	buildStartButtonTexture.loadFromFile("World/builder/buildStart.png");
	buildStartButton.setTexture(buildStartButtonTexture);
	buildStartButton.setPosition(buildStartButton.getTextureRect().width / 8, 10);
	buildStartButton.setScale(screenSize.y / 15 / buildStartButtonTexture.getSize().x, screenSize.y / 15 / buildStartButtonTexture.getSize().y);

	//initialize buildStop button
	buildStopButtonTexture.loadFromFile("World/builder/buildStop.png");
	buildStopButton.setTexture(buildStopButtonTexture);
	buildStopButton.setPosition(buildStopButton.getTextureRect().width / 8, 10);
	buildStopButton.setScale(buildStartButton.getScale());

	//initialize icons of built objects 
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconTexture.loadFromFile(builtObjects[i].icon);
		builtObjects[i].iconSprite.setTexture(builtObjects[i].iconTexture);
		builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getTextureRect().height * 1.25);
		builtObjects[i].iconSprite.setScale(buildStartButton.getScale());
	}

	//initialize frame for recipes
	recipeFrame.setScale((float)builtObjects[0].iconTexture.getSize().x / recipeFrame.getTextureRect().width * 3,
		(float)builtObjects[0].iconTexture.getSize().y / recipeFrame.getTextureRect().height * 2);
}

void Builder::inventoryBounding(std::vector<std::pair <int, int>>& inventory)
{
	currentInventory = std::ref(inventory);
}

void Builder::inicializeObjectsInfo()
{
	recipeFrameTexture.loadFromFile("World/builder/recipeFrame.png");
	recipeFrame.setTexture(recipeFrameTexture);

	std::string objectIconPath, objectImagePath, objectImageType;
	std::ifstream fin(builderObjectsInfoFileDirectory);
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

void Builder::draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f focusedObjectPosition, std::vector<WorldObject*> visibleItems)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (!isBuilding)
	{
		window.draw(buildStartButton);
		return;
	}
	
	animator(elapsedTime);

	if (currentObject != -1)
	{
		if (buildingReady)
		{
			//initialize spriteSilhouette
			auto sprite = (&spriteMap[builtObjects[currentObject].image])->sprite;
			auto terrain = dynamic_cast<TerrainObject*>(staticGrid.getItemByName(builtObjects[currentObject].type));

			//Helper::drawText(std::to_string(builtObjects[currentObject].recipe.size()), 30, 200, 200, &window);
			Helper::drawText(std::to_string(currentInventory.get().size()), 30, 200, 200, &window);
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
			Vector2f testPosition = Vector2f(mousePos.x - terrain->getTextureBoxOffset().x*scaleFactor, mousePos.y + (terrain->getTextureBoxSize().y - terrain->getTextureBoxOffset().y)*scaleFactor);
			sprite.setPosition(Vector2f(mousePos.x - terrain->getTextureBoxOffset().x*scaleFactor, mousePos.y + (terrain->getTextureBoxSize().y - terrain->getTextureBoxOffset().y)*scaleFactor));

			window.draw(sprite);
		}
	}

	window.draw(buildStopButton);

	//draw icons
	for (int i = 0; i < builtObjects.size(); i++)
	{
		window.draw(builtObjects[i].iconSprite);
	}

	if (isRecipeFrame)
	{
		window.draw(recipeFrame);
	}
}

void Builder::interact()
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (isBuilding)
	{
		bool f = false;
		for (int i = 0; i < builtObjects.size(); i++)
		{
			if (Helper::isIntersects(mousePos, IntRect(builtObjects[i].iconSprite.getPosition().x, builtObjects[i].iconSprite.getPosition().y, builtObjects[i].iconSprite.getTextureRect().width, builtObjects[i].iconSprite.getTextureRect().height)))
			{
				if (!buildingReady)
					currentObject = i;
				isRecipeFrame = true;
				recipeFrame.setPosition(builtObjects[i].iconSprite.getPosition().x + builtObjects[i].iconSprite.getTextureRect().width*1.25, builtObjects[i].iconSprite.getPosition().y);
				f = true;
				break;
			}
		}
		if (!f)
			isRecipeFrame = false;
	}
}

void Builder::onMouseDownInteract(Vector2f focusedObjectPosition, float scaleFactor)
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
				builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getTextureRect().height*1.25);
			}
		}
		else
		{
			if (isBuilding && buildingReady)
			{
				buildingPosition = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x * scaleFactor) / scaleFactor,
					(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);
				buildingAvaliable = true;
				//buildObject(Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x*scaleFactor) / scaleFactor,
				//(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor), world);
			}
			else
			{
				buildingPosition = Vector2f(-1, -1);
			}

			//can be crafted
			buildingReady = true;
			if (currentObject != -1)
			{
				std::vector<std::pair <int, int>>& temporaryInventory = builtObjects[currentObject].recipe;
				for (auto curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
				{
					for (auto curInvItem = currentInventory.get().begin(); curInvItem != currentInventory.get().end(); ++curInvItem)
					{
						if (curRecipeItem->first == curInvItem->first)
						{
							curRecipeItem->second -= curInvItem->second;
						}
					}
					if (curRecipeItem->second > 0)
						buildingReady = false;
				}
			}
		}
}

void Builder::animator(float elapsedTime)
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

std::string Builder::getBuiltObjectType()
{
	if (currentObject == -1)
		return "-1";

	return builtObjects[currentObject].type;
}
