#include "Deer.h"
#include "Hare.h"

using namespace sf;

Deer::Deer(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (360, 300);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 8e-4;
	animationSpeed = 5e-4;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = conditionalSizeUnits.x * 3;
	morality = 5; // from 1 to 10
	fear = 0;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitself = 10e5;
	timeForNewHitself = timeAfterHitself;
	timeForNewHit = 10e6;
	//routeGenerationAbility = false;

	toSaveName = "deer";
}

Deer::~Deer()
{
}

Vector2i Deer::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Deer::setTarget(DynamicObject& object)
{
	return;
}

void Deer::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Deer::behavior(float elapsedTime)
{

}

Vector2f Deer::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Deer::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Deer::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Deer::prepareSpriteNames(long long elapsedTime)
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
					fullSprite.path = "Game/worldSprites/deer/stand/left/";
					break;
				case right:
					fullSprite.path = "Game/worldSprites/deer/stand/right/";
					break;
				case up:
					fullSprite.path = "Game/worldSprites/deer/stand/up/";
					break;
				case down:
					fullSprite.path = "Game/worldSprites/deer/stand/down/";
					break;
				default:;
			}		
			fullSprite.path += std::to_string(currentSprite) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			fullSprite.path = "Game/worldSprites/deer/stand/down/1.png";
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
				fullSprite.path = "Game/worldSprites/deer/move/left/";				
				break;
			case right:
				fullSprite.path = "Game/worldSprites/deer/move/right/";
				break;
			case up:
				fullSprite.path = "Game/worldSprites/deer/move/up/";
				break;
			case down:
				fullSprite.path = "Game/worldSprites/deer/move/down/";
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