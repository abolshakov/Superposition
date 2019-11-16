#include "Nightmare2.h"

Nightmare2::Nightmare2(std::string objectName, Vector2f centerPosition) : Monster(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(600, 600);
	defaultSpeed = 0.0005f;
	speed = 0.0005f;
	radius = 120;
	hitDistance = 120;
	strength = 17;
	healthPoint = 180;
	currentAction = relax;
	timeForNewHitself = 10e4;
	timeAfterHitself = timeForNewHitself;
	timeForNewHit = 1e6;
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare2_";
	tag = Tag::nightmare2;
}


Nightmare2::~Nightmare2()
{
}


Vector2i Nightmare2::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 6 / 8);
}

void Nightmare2::doAttack(WorldObject* target)
{
	if (timeAfterHit >= timeForNewHit)
	{
		if (Helper::getDist(position, victim->getPosition()) <= (this->radius + victim->getRadius() + hitDistance / 5))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
	}
}

void Nightmare2::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;

	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
	if (side == right)
	{
		sideStr = "left";
		fullSprite.mirrored = true;
	}
	if (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT)
	{
		directionStr = "left";
		fullSprite.mirrored = true;
	}

	switch (currentAction)
	{
	case commonHit:
	{
		animationLength = 12;
		animationSpeed = 12;
		fullSprite.path = "Game/worldSprites/nightmare2/hit/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case combatState:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare2/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case relax:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare2/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/nightmare2/stand/down/1.png";
		currentSprite[0] = 1;
		break;
	}
	case move:
	{
		animationLength = 8;
		fullSprite.path = "Game/worldSprites/nightmare2/move/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		if (currentSprite[0] == 2 || currentSprite[0] == 6)
			doShake = true;
		break;
	}
	default:;
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

