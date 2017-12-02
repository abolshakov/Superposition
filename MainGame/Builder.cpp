#include "Builder.h"

Builder::Builder()
{
	inicializeObjectsInfo();
	buildStartButtonTexture.loadFromFile("World/buildStart.png");
	buildStartButton.setTexture(buildStartButtonTexture);
	buildStartButton.setPosition(buildStartButton.getTextureRect().width / 8, 10);

	buildStopButtonTexture.loadFromFile("World/buildStop.png");
	buildStopButton.setTexture(buildStopButtonTexture);
	buildStopButton.setPosition(buildStopButton.getTextureRect().width / 8, 10);

	recipeFrameTexture.loadFromFile("World/recipeFrame.png");
	recipeFrame.setTexture(recipeFrameTexture);	

	for (int i = 0; i < 10; i++)
	{
		builtObjects[i].iconTexture.loadFromFile(builtObjects[i].icon);
		builtObjects[i].iconSprite.setTexture(builtObjects[i].iconTexture);
		builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getTextureRect().height * 1.25);
	}
	recipeFrame.setScale((float)builtObjects[0].sprite.getTextureRect().width / recipeFrame.getTextureRect().width * 3, (float)builtObjects[0].sprite.getTextureRect().height / recipeFrame.getTextureRect().height * 2);
}
void Builder::draw(RenderWindow &window, World& world, float elapsedTime)
{
	//Helper::drawText(to_string(builtObjects[0].sprite.getTextureRect))
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (!isBuilding)
		window.draw(buildStartButton);
	if (isBuilding)
	{
		animator(elapsedTime);
		auto sprite = (&world.spriteMap[builtObjects[currentObject].image])->sprite;
		auto terrain = dynamic_cast<TerrainObject*>(world.staticGrid.getItemByName(builtObjects[currentObject].type));
		Helper::drawText(to_string(sprite.getTextureRect().height), 50, 200, 100, &window);		
		sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
		sprite.setScale(terrain->getScaleRatio().x*world.scaleFactor, terrain->getScaleRatio().y*world.scaleFactor*sqrt(sqrt(world.scaleFactor)));

		buildingAvaliable = true;
		Vector2f mouseWorldPos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + world.focusedObject->getPosition().x*world.scaleFactor) / world.scaleFactor,
			(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + world.focusedObject->getPosition().y*world.scaleFactor) / world.scaleFactor);
		for (auto item : world.visibleItems)
		{
			auto terrain = dynamic_cast<TerrainObject*>(item);
			if (terrain)
				if (Helper::isIntersectTerrain(mouseWorldPos, *terrain, terrain->intersectsRadius))
				{
					sprite.setColor(Color::Red);
					buildingAvaliable = false;
					break;
				}
		}
		if (buildingAvaliable)
			sprite.setColor(Color::Green);
		sprite.setPosition(Vector2f(mousePos.x - terrain->getTextureBoxOffset().x*world.scaleFactor, mousePos.y + (terrain->getTextureBoxSize().y - terrain->getTextureBoxOffset().y)*world.scaleFactor));
		window.draw(sprite);

		window.draw(buildStopButton);

		for (int i = 0; i < 10; i++)
		{
			window.draw(builtObjects[i].iconSprite);
		}
	}
	if (isRecipeFrame)
		window.draw(recipeFrame);
}
void Builder::interact()
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (isBuilding)
	{
		bool f = false;
		for (int i = 0; i < 10; i++)
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
			for (int i = 0; i < 10; i++)
			{
				builtObjects[i].iconSprite.setPosition(builtObjects[0].iconSprite.getTextureRect().width*-1, (i + 1)*builtObjects[i].iconSprite.getTextureRect().height*1.25);
			}
		}
		else
		{
			bool f = false;
			for (int i = 0; i < 10; i++)
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
					inicialize(Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + world.focusedObject->getPosition().x*world.scaleFactor) / world.scaleFactor,
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
		for (int i = 0; i < 10; i++)
		{
			builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getTextureRect().width / 8 + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
		}
		exit;
	}
	for (int i = 0; i < 10; i++)
	{
		builtObjects[i].iconSprite.setPosition(builtObjects[i].iconSprite.getPosition().x + animationSpeed * elapsedTime, builtObjects[i].iconSprite.getPosition().y);
	}
}
void Builder::inicializeObjectsInfo()
{
	for (int i = 0; i < 10; i++)
	{
		builtObjects[i].icon = "World/Tree.png";
		builtObjects[i].image = "roseTree3.png";
		builtObjects[i].type = "roseTree";
	}
}
void Builder::inicialize(Vector2f position, World& world)
{
	if (builtObjects[currentObject].type == "roseTree")
	{
		world.initializeRoseTree(position, 3, "");
	}
}

