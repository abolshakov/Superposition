#include "BuildSystemMaker.h"
#include "ObjectInitializer.h"
#include "DroppedLoot.h"

BuildSystemMaker::BuildSystemMaker()
{

}

BuildSystemMaker::~BuildSystemMaker()
{

}

void BuildSystemMaker::Init(const std::unordered_map<Tag, CellSprite>& itemsSpriteList)
{
	this->craftIngredientsSpriteList = itemsSpriteList;
	inicializeObjectsInfo();
	initializeButtons();

	font.loadFromFile("Fonts/Bebas.ttf");
	numberOfObjects.setFont(font);
	numberOfObjects.setCharacterSize(unsigned int(30 * Helper::GetScreenSize().y / 1440));
	numberOfObjects.setFillColor(Color(200, 200, 200, 255));
}

void BuildSystemMaker::initializeButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

	//initialize buildStart button
	buildStartButtonTexture.loadFromFile("Game/buildSystem/buildStart.png");
	buildStartButton.setTexture(buildStartButtonTexture);
	buildStartButton.setPosition(buildStartButton.getTextureRect().width / float(8), float(10));
	buildStartButton.setScale(screenSize.y / 15 / buildStartButtonTexture.getSize().x, screenSize.y / 15 / buildStartButtonTexture.getSize().y);

	//initialize buildStop button
	buildStopButtonTexture.loadFromFile("Game/buildSystem/buildStop.png");
	buildStopButton.setTexture(buildStopButtonTexture);
	buildStopButton.setPosition(buildStopButton.getTextureRect().width / float(8), float(10));
	buildStopButton.setScale(buildStartButton.getScale());

	//initialize icons of built objects 
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconTexture.loadFromFile(builtObjects[i].icon);
		builtObjects[i].iconSprite.setTexture(builtObjects[i].iconTexture);		
		builtObjects[i].iconSprite.setScale(buildStartButton.getScale());
		builtObjects[i].iconSprite.setPosition(float(builtObjects[0].iconSprite.getTextureRect().width*-1), float((i + 1)*builtObjects[i].iconSprite.getGlobalBounds().height * 1.25));
	}

	//initialize frame for recipes
	recipeFrame.setScale((float)builtObjects[0].iconTexture.getSize().x / recipeFrame.getTextureRect().width * 3,
		(float)builtObjects[0].iconTexture.getSize().y / recipeFrame.getTextureRect().height * 2);
}

void BuildSystemMaker::inventoryBounding(std::vector<HeroBag>* boundBags)
{
	this->boundBags = boundBags;
}

void BuildSystemMaker::inicializeObjectsInfo()
{
	//recipeFrameTexture.loadFromFile("Game/BuildSystemMaker/recipeFrame.png");
	//recipeFrame.setTexture(recipeFrameTexture);

	std::string objectIconPath, objectImageType;
	std::ifstream fin(BuildSystemMakerObjectsInfoFileDirectory);
	while (fin >> objectIconPath >> objectImageType)
	{
		objectInfo infoItem;

		infoItem.icon = objectIconPath;
		infoItem.type = objectImageType;

		int recipeItemId, recipeItemAmount, recipeLength;

		fin >> recipeLength;
		for (int i = 0; i < recipeLength; i++)
		{
			fin >> recipeItemId >> recipeItemAmount;
			infoItem.recipe.push_back(std::make_pair(Tag(recipeItemId), recipeItemAmount));
		}

		builtObjects.push_back(infoItem);
	}

	fin.close();
}

void BuildSystemMaker::draw(RenderWindow &window, float elapsedTime, std::unordered_map<std::string, BoardSprite>& spriteMap, GridList<StaticObject>& staticGrid, float scaleFactor, Vector2f cameraPosition, std::vector<WorldObject*> visibleItems, bool showPositioning)
{
	Vector2f mousePos = (Vector2f )Mouse::getPosition();
	mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	if (showPositioning && selectedObject != Tag::emptyCell)
	{
		//initialize spriteSilhouette	
		StaticObject* terrain = nullptr;
		int type = 1;
		if (droppedLootIdList.count(selectedObject) > 0)
			terrain = ObjectInitializer::initializeStaticItem(Tag::droppedLoot, Vector2f(0, 0), int(selectedObject), "", 1, cameraPosition, scaleFactor, &spriteMap);
		else
		{
			if (selectedObject == Tag::totem)
			{
				for (auto& item : visibleItems)
				{
					if (staticGrid.getIndexByPoint(item->getPosition().x, item->getPosition().y) != staticGrid.getIndexByPoint(mouseWorldPos.x, mouseWorldPos.y))
						continue;
					bool match = false;
					auto droppedLoot = dynamic_cast<DroppedLoot*>(item);
					if (droppedLoot && droppedLoot->getType() == 201)
					{
						for (auto& cell : droppedLoot->inventory)
							if (cell.first == Tag::hare)
							{
								match = true;
								break;
							}
					}
					if (match)
					{
						type = 2;
						break;
					}
				}
			}
			terrain = ObjectInitializer::initializeStaticItem(selectedObject, Vector2f(0, 0), type, "", 1, cameraPosition, scaleFactor, &spriteMap);
		}		

		const Vector2f terrainPos = terrain->getBuildPosition(visibleItems, scaleFactor, cameraPosition);		

		terrain->prepareSpriteNames(0);
		auto sprite = (&spriteMap[(terrain->additionalSprites)[0].path])->sprite;
		sprite.setOrigin(float(sprite.getTextureRect().left), float(sprite.getTextureRect().top + sprite.getTextureRect().height));
		sprite.setScale(terrain->getScaleRatio().x*scaleFactor, terrain->getScaleRatio().y*scaleFactor*sqrt(sqrt(scaleFactor)));

		if (terrainPos != Vector2f (-1, -1))
		{
			spriteBuildPos = terrainPos;
			sprite.setPosition((terrainPos.x - cameraPosition.x) * scaleFactor + Helper::GetScreenSize().x / 2 - terrain->getTextureOffset().x*scaleFactor, (terrainPos.y - cameraPosition.y) * scaleFactor + Helper::GetScreenSize().y / 2 + (terrain->getTextureSize().y - terrain->getTextureOffset().y)*scaleFactor);
		}
		else
		{
			spriteBuildPos = Vector2f(mouseWorldPos);
			sprite.setPosition(Vector2f (mousePos.x - terrain->getTextureOffset().x*scaleFactor, mousePos.y + (terrain->getTextureSize().y - terrain->getTextureOffset().y)*scaleFactor));
		}

		canBePlaced = true;

		if (staticGrid.isIntersectWithOthers(spriteBuildPos, float(terrain->getRadius()), visibleItems, terrain->isDotsAdjusted))
		{
			sprite.setColor(Color(255,99,71));
			canBePlaced = false;
		}
		else
			sprite.setColor(Color(127, 255, 0));

		buildType = terrain->getType();
		delete terrain;
		if (!canBePlaced)
		{
			buildType = 1;
			spriteBuildPos = Vector2f (-1, -1);
		}

		window.draw(sprite);
	}

	//window.draw(buildStopButton);	

	//draw icons
	for (int i = 0; i < builtObjects.size(); i++)
	{
		window.draw(builtObjects[i].iconSprite);
	}

	drawRecipeFrame(window);
}

void BuildSystemMaker::drawRecipeFrame(RenderWindow &window)
{
	if (isRecipeFrame)
	{
		window.draw(recipeFrame);
		for (int i = 0; i < builtObjects[currentObject].recipe.size(); i++)
		{
			Sprite *currentRecipeItem = &craftIngredientsSpriteList[Tag(builtObjects[currentObject].recipe[i].first)].sprite;
			currentRecipeItem->setPosition(recipeFrame.getPosition().x + i * currentRecipeItem->getGlobalBounds().width, recipeFrame.getPosition().y);
			Vector2f scale = buildStartButton.getScale();
			currentRecipeItem->setScale(scale);

			window.draw(*currentRecipeItem);

			int boundInventoryObjectsCount = 0;
			for (auto bag = boundBags->begin(); bag != boundBags->end(); bag++)
			{
				for (auto& item : bag->cells)
				{
					if (item.content.first == builtObjects[currentObject].recipe[i].first)
						boundInventoryObjectsCount += item.content.second;
				}
			}

			numberOfObjects.setString(std::to_string(boundInventoryObjectsCount) + '/' + std::to_string(builtObjects[currentObject].recipe[i].second));
			if (boundInventoryObjectsCount < builtObjects[currentObject].recipe[i].second)
				numberOfObjects.setFillColor(Color(193, 63, 63, 255));
			numberOfObjects.setPosition(currentRecipeItem->getPosition().x + currentRecipeItem->getGlobalBounds().width - numberOfObjects.getGlobalBounds().width,
				currentRecipeItem->getPosition().y + currentRecipeItem->getGlobalBounds().height - numberOfObjects.getGlobalBounds().height);
			window.draw(numberOfObjects);
			numberOfObjects.setFillColor(Color(200, 200, 200, 255));
		}

	}
}

void BuildSystemMaker::interact()
{
	Vector2f mousePos = (Vector2f )Mouse::getPosition();
	bool f = false;
	for (int i = 0; i < builtObjects.size(); i++)
	{
		if (Helper::isIntersects(mousePos, IntRect(builtObjects[i].iconSprite.getPosition().x, builtObjects[i].iconSprite.getPosition().y, builtObjects[i].iconSprite.getGlobalBounds().width, builtObjects[i].iconSprite.getGlobalBounds().height)))
		{
			currentObject = i;
			isRecipeFrame = true;
			recipeFrame.setPosition(float(builtObjects[i].iconSprite.getPosition().x + builtObjects[i].iconSprite.getGlobalBounds().width*1.25f), float(builtObjects[i].iconSprite.getPosition().y));
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

void BuildSystemMaker::onMouseUp(Vector2f focusedObjectPosition, float scaleFactor)
{
	Vector2f mousePos = (Vector2f )Mouse::getPosition();

	usedMouse = false;

	if (Helper::isIntersects(mousePos, sf::IntRect(buildStartButton.getGlobalBounds())))
	{
		currentObject = -1;
		usedMouse = true;
	}
	else
		if (Helper::isIntersects(mousePos, sf::IntRect(buildStopButton.getGlobalBounds())))
		{
			animationSpeed = 0.001f;
			for (int i = 0; i < builtObjects.size(); i++)
			{
				builtObjects[i].iconSprite.setPosition(float(builtObjects[0].iconSprite.getGlobalBounds().width*-1), float((i + 1)*builtObjects[i].iconSprite.getGlobalBounds().height * 1.25f));
			}
			usedMouse = true;
			return;
		}

	if (selectedObject != Tag::emptyCell || currentObject != -1)
		usedMouse = true;
	//return;

	if (selectedObject != Tag::emptyCell && currentObject == -1 && canBePlaced)
	{
		if (spriteBuildPos != Vector2f (-1, -1))
			buildingPosition = spriteBuildPos;
		else
			buildingPosition = Vector2f ((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x * scaleFactor) / scaleFactor,
				(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);
	}
	else
		buildingPosition = Vector2f (-1, -1);
}

void BuildSystemMaker::buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor)
{
	if (heldItem->first == Tag::emptyCell)
	{
		buildingPosition = Vector2f (-1, -1);
		return;
	}

	if (canBePlaced)
	{
		buildingPosition = Vector2f ((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + focusedObjectPosition.x * scaleFactor) / scaleFactor,
			(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + focusedObjectPosition.y*scaleFactor) / scaleFactor);
	}
}

bool BuildSystemMaker::canAfford()
{
	if (currentObject != -1)
	{
		std::vector<std::pair <Tag, int>> temporaryInventory = builtObjects[currentObject].recipe;

		for (auto&curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
		{
			for (auto bag = boundBags->begin(); bag != boundBags->end(); bag++)
			{
				bool isBreak = false;
				for (auto&item : bag->cells)
				{
					if (curRecipeItem->first == item.content.first)
					{
						if (item.content.second >= curRecipeItem->second)
						{
							curRecipeItem->second = 0;
							isBreak = true;
							break;
						}
					}
					else
						curRecipeItem->second -= item.content.second;
				}
				if (isBreak)
					break;
			}
			if (curRecipeItem->second > 0)
				return false;
		}

		return true;
	}
	return false;
}

void BuildSystemMaker::wasPlaced()
{
	//std::vector<std::pair <Tag, int>> temporaryInventory = builtObjects[selectedObject].recipe;

	/*for (auto&curRecipeItem = temporaryInventory.begin(); curRecipeItem != temporaryInventory.end(); ++curRecipeItem)
	{
		for (auto bag = boundBags->begin(); bag != boundBags->end(); bag++)
		{
			for (auto& item : bag->cells)
			{
				if (curRecipeItem->first == item.content.first)
				{
					if (item.content.second >= curRecipeItem->second)
					{
						item.content.second -= curRecipeItem->second;
						if (item.content.second == 0)
							item.content.first = Tag::emptyCell;
						curRecipeItem->second = 0;
					}
					else
					{
						curRecipeItem->second -= item.content.second;
						item.content.second = 0;
						item.content.first = Tag::emptyCell;
					}
				}
			}
		}
	}*/

	selectedObject = Tag::emptyCell;
	buildingPosition = Vector2f (-1, -1);
}

void BuildSystemMaker::clearHareBags(int block, GridList<StaticObject>& staticGrid, std::vector<WorldObject*>* visibleItems)
{
	for (auto& item : *visibleItems)
	{
		if (staticGrid.getIndexByPoint(item->getPosition().x, item->getPosition().y) != staticGrid.getIndexByPoint(mouseWorldPos.x, mouseWorldPos.y))
			continue;
		bool match = false;
		auto droppedLoot = dynamic_cast<DroppedLoot*>(item);
		if (droppedLoot && droppedLoot->getType() == 201)
		{
			for (auto& cell : droppedLoot->inventory)
				if (cell.first == Tag::hare)
				{
					match = true;
					break;
				}
		}
		if (match)
		{
			item->deletePromiseOn();
			break;
		}
	}
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
	}
	for (int i = 0; i < builtObjects.size(); i++)
	{
		builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getPosition().x + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
	}
}
