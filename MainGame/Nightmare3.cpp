#include "Nightmare3.h"

Nightmare3::Nightmare3(std::string objectName, Vector2f centerPosition) : Monster(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(600, 600);
	defaultSpeed = 0.0003f;
	speed = 0.0005f;
	radius = 120;
	hitDistance = 120;
	strength = 25;
	healthPoint = 300;
	currentAction = relax;
	timeForNewHitself = 10e4;
	timeAfterHitself = timeForNewHitself;
	timeForNewHit = 1e6;
	timeAfterHit = timeForNewHit;
	toSaveName = "nightmare2_";
	tag = Tag::nightmare2;
}


Nightmare3::~Nightmare3()
{
}


Vector2i Nightmare3::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 6 / 8);
}

void Nightmare3::doAttack(WorldObject* target)
{
	if (timeAfterHit >= timeForNewHit)
	{
		if (Helper::getDist(position, victim->getPosition()) <= (this->radius + victim->getRadius() + hitDistance / 3))
		{
			changeAction(commonHit, true, false);
			timeAfterHit = 0;
		}
		else
			if (Helper::getDist(position, victim->getPosition()) > this->radius + victim->getRadius() + hitDistance * 2)
			{
				changeAction(directHit, true, false);
				timeAfterHit = 0;
			}
		
	}	
}

void Nightmare3::endingPreviousAction()
{
	if (lastAction == combatState)
		changeAction(relax, true, false);
	if (lastAction == commonHit)
		changeAction(combatState, true, false);	
	if (lastAction == directHit)	
		changeAction(relax, true, false);	
	lastAction = relax;
}

void Nightmare3::onSpriteChange()
{
	if (currentAction == directHit && currentSprite[0] == 5)
	{
		birthDynamicInfo whirl;
		whirl.position = position;
		whirl.tag = Tag::clapWhirl;
		whirl.owner = this;
		birthDynamics.push(whirl);
	}
}

void Nightmare3::prepareSpriteNames(long long elapsedTime, float scaleFactor)
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
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/hit/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		if (currentSprite[0] == 6)
			doShake = true;
		break;
	}
	case directHit:
	{
		animationLength = 9;		
		fullSprite.path = "Game/worldSprites/nightmare3/clap/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		if (currentSprite[0] == 6)
			doShake = true;		
		break;
	}
	case combatState:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case relax:
	{
		animationLength = 9;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/nightmare3/stand/down/1.png";
		currentSprite[0] = 1;
		break;
	}
	case move:
	{
		animationLength = 6;
		fullSprite.path = "Game/worldSprites/nightmare3/move/" + sideStr + '/';
		fullSprite.path += std::to_string(currentSprite[0]) + ".png";		
		break;
	}
	default:;
	}

	additionalSprites.push_back(fullSprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		currentSprite[0]++;
		onSpriteChange();
		timeForNewSprite = 0;

		if (currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			if (currentAction == move)
				currentSprite[0] = 4;
			else
				currentSprite[0] = 1;
		}
	}
}

