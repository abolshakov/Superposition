#include "Wolf.h"

using namespace sf;

Wolf::Wolf(std::string objectName, Vector2f centerPosition) : Creature(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2f(250, 200);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationSpeed = 0.0006f;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = Helper::GetScreenSize().y * 1 / 2; 
	morality = 5; // from 1 to 10
	fear = 0;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitself = 100000;
	timeForNewHitself = timeAfterHitself;
	timeForNewHit = 1000000;
	inventoryCapacity = 5;

	inventory.push_back(std::make_pair(1, 2));
	inventory.push_back(std::make_pair(2, 1));
	inventory.push_back(std::make_pair(1, 2));
	inventory.push_back(std::make_pair(2, 2));

	toSaveName = "wolf";
}

Wolf::~Wolf()
{
}

void Wolf::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Wolf::behavior(float elapsedTime)
{

}

Vector2i Wolf::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

std::string Wolf::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
	case bottomHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case upperHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case directHit:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Wolf/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Wolf/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Wolf/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Wolf/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case combatState:
	{
		animationLength = 8;
		switch (side)
		{
			case up:
			{
				spriteName = "Wolf/move/up/";
				break;
			}
			case right:
			{
				spriteName = "Wolf/move/right/";
				break;
			}
			case down:
			{
				spriteName = "Wolf/move/down/";
				break;
			}
			case left:
			{
				spriteName = "Wolf/move/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
	    spriteName = "Wolf/stand/down/";				
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		spriteName = "Wolf/stand/down/1.png";
		currentSprite = 0;
	}
	}

	if (currentAction == move)
	{
		animationLength = 8;
		switch (direction)
		{
		case LEFT:
			spriteName = "Wolf/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case RIGHT:
			spriteName = "Wolf/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "Wolf/move/up/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "Wolf/move/down/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "Wolf/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPRIGHT:
			spriteName = "Wolf/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNLEFT:
			spriteName = "Wolf/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNRIGHT:
			spriteName = "Wolf/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		default:;
		}
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			if (currentAction >= (Actions)(0) && currentAction < (Actions)3)
			{
				lastAction = currentAction;
				currentAction = combatState;
			}
			currentSprite = 1;
		}
	}

	return spriteName;
}