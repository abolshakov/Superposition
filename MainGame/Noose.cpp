#include "Noose.h"

using namespace sf;

Noose::Noose(const std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (360, 300);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 8e-4;
	animationSpeed = 5e-4;
	animationLength = 8;
	radius = 70;
	currentAction = relax;
	//routeGenerationAbility = false;

	toSaveName = "deer";
}

Noose::~Noose()
{
}

Vector2i Noose::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Noose::setTarget(DynamicObject& object)
{
	return;
}

void Noose::behaviorWithDynamic(DynamicObject& target, float elapsedTime)
{
    return;
}

void Noose::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Noose::behavior(float elapsedTime)
{

}

Vector2f Noose::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Noose::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Noose::stopping(bool doStand, bool forgetSelectedTarget)
{
	if (doStand)
	{
		this->movePosition = { -1, -1 };
		this->direction = STAND;
	}

	if (forgetSelectedTarget && selectedTarget != nullptr)
	{
		selectedTarget->isProcessed = false;
		selectedTarget = nullptr;
	}
}

void Noose::endingPreviousAction()
{
	if (lastAction == move)
	{
		currentAction = relax;
        delatePromiseOn();
	}
}

void Noose::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	stopping(false, true);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 700000;
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 500;
	currentSprite = 1;

	movePosition = Vector2f(position.x + cos(direction * pi / 180) * jerkDistance, position.y - sin(direction * pi / 180) * jerkDistance);
}

void Noose::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement ropeSprite, loopSprite;

	ropeSprite.offset = Vector2f(this->textureBoxOffset);
	ropeSprite.size = Vector2f(this->conditionalSizeUnits);
    loopSprite.offset = Vector2f(this->textureBoxOffset);
	loopSprite.size = Vector2f(this->conditionalSizeUnits);

	additionalSprites.clear();

	switch (currentAction)
	{
		case relax:
		{

			ropeSprite.path = "Game/worldSprites/noose/nooseRope.png";
            loopSprite.path = "Game/worldSprites/noose/nooseLoop.png";								
			break;
		}
		case dead:
		{
			animationLength = 1;
			ropeSprite.path = "Game/worldSprites/noose/nooseRope.png";
            loopSprite.path = "Game/worldSprites/noose/nooseLoop.png";					
			currentSprite = 0;
		}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 5;
		ropeSprite.path = "Game/worldSprites/noose/nooseRope.png";
        loopSprite.path = "Game/worldSprites/noose/nooseLoop.png";					
	}

	additionalSprites.push_back(ropeSprite);
    additionalSprites.push_back(loopSprite);

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