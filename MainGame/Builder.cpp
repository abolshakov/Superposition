#include "Builder.h"

Builder::Builder(std::unordered_map<int, cell> itemsSpriteList)
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

		builtObjects.push_back(infoItem);
	}
}

void Builder::draw(RenderWindow &window, World& world, float elapsedTime)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (!isBuilding)
	{
		window.draw(buildStartButton);
		return;
	}

	animator(elapsedTime);
	//initialize spriteSilhouette
	auto sprite = (&world.spriteMap[builtObjects[currentObject].image])->sprite;
	Helper::drawText(std::to_string((&world.spriteMap[builtObjects[currentObject].image])->sprite.getTextureRect().width), 30, 200, 200, &window);
	auto terrain = dynamic_cast<TerrainObject*>(world.staticGrid.getItemByName(builtObjects[currentObject].type));
	sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
	sprite.setScale(terrain->getScaleRatio().x*world.scaleFactor, terrain->getScaleRatio().y*world.scaleFactor*sqrt(sqrt(world.scaleFactor)));
	buildingAvaliable = true;
	Vector2f mouseWorldPos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + world.focusedObject->getPosition().x*world.scaleFactor) / world.scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + world.focusedObject->getPosition().y*world.scaleFactor) / world.scaleFactor);
	//enumeration of intersecting elements
	for (auto item : world.visibleItems)
	{
		if (item->isTerrain)
			continue;
		auto currentTerrain = dynamic_cast<TerrainObject*>(item);
		if (currentTerrain)
			if (Helper::isIntersectTerrain(mouseWorldPos, *currentTerrain, terrain->intersectsRadius))
			{
				sprite.setColor(Color::Red);
				buildingAvaliable = false;
				break;
			}
	}
	if (buildingAvaliable)
		sprite.setColor(Color::Green);
	Vector2f testPosition = Vector2f(mousePos.x - terrain->getTextureBoxOffset().x*world.scaleFactor, mousePos.y + (terrain->getTextureBoxSize().y - terrain->getTextureBoxOffset().y)*world.scaleFactor);
	Helper::drawText(std::to_string(testPosition.x), 30, 200, 250, &window);
	Helper::drawText(std::to_string(testPosition.x), 30, 200, 300, &window);
	sprite.setPosition(Vector2f(mousePos.x - terrain->getTextureBoxOffset().x*world.scaleFactor, mousePos.y + (terrain->getTextureBoxSize().y - terrain->getTextureBoxOffset().y)*world.scaleFactor));

	window.draw(sprite);
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

void Builder::onMouseDownInteract(World& world)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();

	if (Helper::isIntersects(mousePos, buildStartButton.getTextureRect()) && !isBuilding)
	{
		isBuilding = true;
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
			bool f = false;
			for (int i = 0; i < builtObjects.size(); i++)
			{
				if (Helper::isIntersects(mousePos, IntRect(builtObjects[i].iconSprite.getPosition().x, builtObjects[i].iconSprite.getPosition().y, builtObjects[i].iconSprite.getTextureRect().width, builtObjects[i].iconSprite.getTextureRect().height)))
				{
					currentObject = i;
					f = true;
					break;
				}
			}
			if (!f)
				if (isBuilding && buildingAvaliable)
				{
					buildObject(Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + world.focusedObject->getPosition().x*world.scaleFactor) / world.scaleFactor,
						(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + world.focusedObject->getPosition().y*world.scaleFactor) / world.scaleFactor), world);
				}
		}
}

void Builder::setCharacterBuildActivity(Deerchant& hero)
{
	hero.isBuilder = isBuilding;
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

void Builder::buildObject(Vector2f position, World& world)
{
	if (builtObjects[currentObject].type == "homeCosiness")
	{
		world.initializeHomeCosiness(position, 1, "");
	}
	if (builtObjects[currentObject].type == "bonefireOfInsight")
	{
		world.initializeBonefireOfInsight(position, 1, "");
	}
}