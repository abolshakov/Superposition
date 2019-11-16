#include "Bear.h"

using namespace sf;

Bear::Bear(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(432, 384);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	defaultSpeed = 0.0003f;
	speed = 0.0003f;
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
	toSaveName = "bear";
	tag = Tag::bear;
}

Bear::~Bear()
{
}

Vector2i Bear::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Bear::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void Bear::behavior(float elapsedTime)
{
	endingPreviousAction();
	fightLogic(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}

	if (this->owner != nullptr)
	{
		speed = defaultSpeed;
		if (currentAction == commonHit)
		{
			movePosition = position;
			return;
		}
		side = calculateSide(owner->getPosition(), elapsedTime);
		if (Helper::getDist(position, owner->getPosition()) > sightRange / 2)
		{
			changeAction(grab, false, false);
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

	side = calculateSide(movePosition, elapsedTime);

	if (boundTarget == nullptr)
		return;
	const float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());
	speed = std::max(defaultSpeed, (defaultSpeed * 10) * (1 - (distanceToTarget) / sightRange * 1.5f));
	//animationSpeed = std::max(0.0003f, 0.0003f * speed / defaultSpeed);

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

Vector2f Bear::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Bear::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Bear::endingPreviousAction()
{
	if (lastAction == commonHit)
		currentAction = relax;

	lastAction = relax;
}

void Bear::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Bear::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
	if (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT)
	{
		directionStr = "left";
		fullSprite.mirrored = true;
	}

	switch (currentAction)
	{
	case relax:
	{
		animationLength = 1;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case commonHit:
	{
		animationLength = 6;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/hunt/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/stand/down/1.png";
		currentSprite[0] = 1;
	}
	default:;
	}

	if (currentAction == move || currentAction == grab)
	{
		animationLength = 7;
		animationSpeed = 10;
		fullSprite.path = "Game/worldSprites/bear/move/" + directionStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}
}