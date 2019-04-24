#include "Deer.h"
#include "Hare.h"

using namespace sf;

Deer::Deer(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (360, 300);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	//speed = 7e-4;
	speed = 2e-4;
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
	id = deer;
	toSaveName = "deer";
	tag = deerTag;
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

void Deer::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void Deer::behavior(float elapsedTime)
{
	endingPreviousAction();
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}
	
	if (this->owner != nullptr)
	{		
		routeGenerationAbility = false;
		if (currentAction == commonHit)
		{
			movePosition = position;
			return;			
		}
		setSide(owner->getPosition(), elapsedTime);
		if (Helper::getDist(position, owner->getPosition()) > sightRange / 2)
		{
			changeAction(grab, true, false);
			movePosition = owner->getPosition();
		}
		else
			if (Helper::getDist(position, owner->getPosition()) <= sightRange / 2.5 && currentAction != relax)
			{
				changeAction(relax, true, false);
				movePosition = position;		
			}
		return;
	}

	routeGenerationAbility = true;
	setSide(movePosition, elapsedTime);

	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());

	if (distanceToTarget <= sightRange)
	{
		changeAction(move, false, true);
		movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
	}
	else
	{
		if (currentAction == move)
		{
			if (distanceToTarget >= sightRange * 1.5)
			{
				changeAction(relax, true, true);
				direction = STAND;
				movePosition = { -1, -1 };
			}
			else
				movePosition = Vector2f(position.x - (boundTarget->getPosition().x - position.x), position.y - (boundTarget->getPosition().y - position.y));
		}
	}
	distanceToNearest = 10e6;
	boundTarget = nullptr;
}

Vector2f Deer::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Deer::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Deer::endingPreviousAction()
{
	if (lastAction == commonHit)	
		currentAction = relax;

	lastAction = relax;
}

void Deer::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

Vector2f Deer::getHeadPosition()
{
	Vector2f upperLeft = Vector2f(position.x - textureBoxOffset.x, position.y - textureBoxOffset.y);

	if ((side == up && movePosition == position) || (movePosition != position && direction == UP))
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.569, upperLeft.y + conditionalSizeUnits.y * 0.111);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.575, upperLeft.y + conditionalSizeUnits.y * 0.155);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.577, upperLeft.y + conditionalSizeUnits.y * 0.202);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.580, upperLeft.y + conditionalSizeUnits.y * 0.150);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.570, upperLeft.y + conditionalSizeUnits.y * 0.106);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.570, upperLeft.y + conditionalSizeUnits.y * 0.093);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.575, upperLeft.y + conditionalSizeUnits.y * 0.075);
	}
	if ((side == down && movePosition == position) || (movePosition != position && direction == DOWN))
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.446, upperLeft.y + conditionalSizeUnits.y * 0.222);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437, upperLeft.y + conditionalSizeUnits.y * 0.243);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437, upperLeft.y + conditionalSizeUnits.y * 0.258);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.437, upperLeft.y + conditionalSizeUnits.y * 0.242);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.440, upperLeft.y + conditionalSizeUnits.y * 0.168);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.444, upperLeft.y + conditionalSizeUnits.y * 0.150);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.445, upperLeft.y + conditionalSizeUnits.y * 0.182);
	}
	if ((side == left && movePosition == position) || (movePosition != position && direction == LEFT || direction == UPLEFT || direction == DOWNLEFT))
	{
		if (currentSprite[0] == 1)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.313, upperLeft.y + conditionalSizeUnits.y * 0.147);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.325, upperLeft.y + conditionalSizeUnits.y * 0.159);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.338, upperLeft.y + conditionalSizeUnits.y * 0.169);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.319, upperLeft.y + conditionalSizeUnits.y * 0.138);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.287, upperLeft.y + conditionalSizeUnits.y * 0.123);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.282, upperLeft.y + conditionalSizeUnits.y * 0.130);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.277, upperLeft.y + conditionalSizeUnits.y * 0.138);
	}
	if ((side == right && movePosition == position) || (movePosition != position && direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT))
	{	
		if (currentSprite[0] == 1)		
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.6, upperLeft.y + conditionalSizeUnits.y * 0.17);
		if (currentSprite[0] == 2)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.59, upperLeft.y + conditionalSizeUnits.y * 0.19);
		if (currentSprite[0] == 3)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.56, upperLeft.y + conditionalSizeUnits.y * 0.2);
		if (currentSprite[0] == 4)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.59, upperLeft.y + conditionalSizeUnits.y * 0.17);
		if (currentSprite[0] == 5)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.62, upperLeft.y + conditionalSizeUnits.y * 0.16);
		if (currentSprite[0] == 6)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.63, upperLeft.y + conditionalSizeUnits.y * 0.16);
		if (currentSprite[0] == 7)
			return Vector2f(upperLeft.x + conditionalSizeUnits.x * 0.63, upperLeft.y + conditionalSizeUnits.y * 0.17);
	}
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
			animationSpeed = 5e-4;
			fullSprite.path = "Game/worldSprites/deer/stand/" + DynamicObject::sideToString(side) + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case commonHit:
		{
			animationLength = 6;
			animationSpeed = 25e-5;
			fullSprite.path = "Game/worldSprites/deer/hunt/" + DynamicObject::sideToString(side) + '/';
			fullSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			animationSpeed = 5e-4;
			fullSprite.path = "Game/worldSprites/deer/stand/down/1.png";
			currentSprite[0] = 1;
		}
	default:;
	}

	if (currentAction == move || currentAction == grab)
	{
		animationLength = 7;
		animationSpeed = 5e-4;
		fullSprite.path = "Game/worldSprites/deer/move/" + DynamicObject::directionToString(direction) + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}
}