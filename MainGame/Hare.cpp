#include "Hare.h"

using namespace sf;

Hare::Hare(std::string objectName, Vector2f centerPosition) : Creature(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2f(180, 150);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationSpeed = 0.0005f;
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

	toSaveName = "hare";
}

Hare::~Hare()
{
}

void Hare::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Hare::behavior(float elapsedTime)
{

}

Vector2i Hare::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

std::string Hare::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
	case bottomHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			spriteName = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			spriteName = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			spriteName = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			spriteName = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case upperHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			spriteName = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			spriteName = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			spriteName = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			spriteName = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case directHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			spriteName = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			spriteName = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			spriteName = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			spriteName = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case combatState:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			spriteName = "Game/worldSprites/hare/move/up/";
			break;
		}
		case right:
		{
			spriteName = "Game/worldSprites/hare/move/right/";
			break;
		}
		case down:
		{
			spriteName = "Game/worldSprites/hare/move/down/";
			break;
		}
		case left:
		{
			spriteName = "Game/worldSprites/hare/move/left/";
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
		spriteName = "Game/worldSprites/hare/stand/down/";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		spriteName = "Game/worldSprites/hare/stand/down/1.png";
		currentSprite = 0;
	}
	}

	if (currentAction == move)
	{
		animationLength = 5;
		switch (direction)
		{
		case LEFT:
			spriteName = "Game/worldSprites/hare/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case RIGHT:
			spriteName = "Game/worldSprites/hare/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "Game/worldSprites/hare/move/up/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "Game/worldSprites/hare/move/down/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "Game/worldSprites/hare/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPRIGHT:
			spriteName = "Game/worldSprites/hare/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNLEFT:
			spriteName = "Game/worldSprites/hare/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNRIGHT:
			spriteName = "Game/worldSprites/hare/move/right/";
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