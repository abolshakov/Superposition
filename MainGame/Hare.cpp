#include "Hare.h"

using namespace sf;

Hare::Hare(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (180, 150);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0006f;
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
	//routeGenerationAbility = false;

	toSaveName = "hare";
}

Hare::~Hare()
{
}

Vector2i Hare::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Hare::setTarget(DynamicObject& object)
{
	return;
}

void Hare::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Hare::behavior(float elapsedTime)
{

}

Vector2f Hare::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Hare::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Hare::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Hare::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();

	switch (currentAction)
	{
		case relax:
		{
			animationLength = 1;
			switch (side)
			{
				case left:
					fullSprite.path = "Game/worldSprites/hare/stand/down/";
					break;
				case right:
					fullSprite.path = "Game/worldSprites/hare/stand/right/";
					break;
				case up:
					fullSprite.path = "Game/worldSprites/hare/stand/up/";
					break;
				case down:
					fullSprite.path = "Game/worldSprites/hare/stand/down/";
					break;
				default:;
			}		
			fullSprite.path += std::to_string(currentSprite) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			fullSprite.path = "Game/worldSprites/hare/stand/down/1.png";
			currentSprite = 0;
		}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 5;
		switch (side)
		{
			case left:
				fullSprite.path = "Game/worldSprites/hare/move/left/";				
				break;
			case right:
				fullSprite.path = "Game/worldSprites/hare/move/right/";
				break;
			case up:
				fullSprite.path = "Game/worldSprites/hare/move/up/";
				break;
			case down:
				fullSprite.path = "Game/worldSprites/hare/move/down/";
				break;
			default:;		
		}
		fullSprite.path += std::to_string(currentSprite) + ".png";
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			if (currentAction >= static_cast<Actions>(0) && currentAction < static_cast<Actions>(3))
			{
				lastAction = currentAction;
				currentAction = combatState;
			}
			currentSprite = 1;
		}
	}
}