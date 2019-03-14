#include "Monster.h"

using namespace sf;

Monster::Monster(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (200, 250);
	timeForNewRoute = 1000000;
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0004f;
	animationSpeed = 0.0005f;
	animationLength = 8;
	radius = 100;
	strength = 100;
	healthPoint = 10;
	currentAction = relax;	
	timeAfterHitself = 100000;
	timeForNewHitself = timeAfterHitself;
	timeForNewHit = 1000000;
	inventoryCapacity = 5;

	inventory.push_back(std::make_pair(1, 2));
	inventory.push_back(std::make_pair(1, 1));
	inventory.push_back(std::make_pair(1, 2));
	inventory.push_back(std::make_pair(2, 2));

	toSaveName = "monster";
}

Monster::~Monster()
{

}

void Monster::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Monster::behavior(float elapsedTime)
{

}

Vector2i Monster::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, textureBox.height * 7 / 8);
}

void Monster::setTarget(DynamicObject& object)
{
	if (object.tag == mainHeroTag)
		return; //targetPosition = object.getPosition();
}

void Monster::behaviorWithDynamic(DynamicObject& target, float elapsedTime)
{
	if (healthPoint <= 0)
	{
		currentAction = dead;
		direction = STAND;
		return;
	}

	if (target.tag != mainHeroTag)
		return;	

	setSide(movePosition, elapsedTime);

	Vector2f curPos = this->getPosition();
	Vector2f tarPos = target.getPosition();

	if (Helper::getDist(curPos, tarPos) <= (this->radius + target.getRadius()))
	{
		if (currentAction == move)
		{
			currentAction = combatState;
			timeAfterHit = timeForNewHit;
		}
		else
		{
			if (currentAction >= 0 && currentAction < 3 && currentSprite == strikingSprite && wasHit == false)
			{
				target.takeDamage(this->strength);
				wasHit = true;
			}
		}
		direction = STAND;
		if (currentAction == combatState)
		{
			wasHit = false;
			timeAfterHit += elapsedTime;
			if (timeAfterHit >= timeForNewHit)
			{
				currentAction = bottomHit;
				currentSprite = 1;
				timeAfterHit = 0;
			}
		}
	}
	else
	{
		if (currentAction != upperHit && currentAction != bottomHit && currentAction != directHit)
		{
			timeAfterHit = 0;
			currentAction = move;
			movePosition = target.getPosition();
			//setMoveOffset(target.getRadius());
		}
	}
}

Vector2f Monster::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Monster::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Monster::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Monster::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();

	switch (currentAction)
	{
	case bottomHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		fullSprite.path += std::to_string(currentSprite);
		fullSprite.path += ".png";
		break;
	}
	case upperHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		fullSprite.path += std::to_string(currentSprite);
		fullSprite.path += ".png";
		break;
	}
	case directHit:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/up/";
			break;
		}
		case right:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/right/";
			break;
		}
		case down:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/down/";
			break;
		}
		case left:
		{
			fullSprite.path = "Game/worldSprites/hare/hit/left/";
			break;
		}
		}
		fullSprite.path += std::to_string(currentSprite);
		fullSprite.path += ".png";
		break;
	}
	case combatState:
	{
		animationLength = 5;
		switch (side)
		{
		case up:
		{
			fullSprite.path = "Game/worldSprites/hare/move/up/";
			break;
		}
		case right:
		{
			fullSprite.path = "Game/worldSprites/hare/move/right/";
			break;
		}
		case down:
		{
			fullSprite.path = "Game/worldSprites/hare/move/down/";
			break;
		}
		case left:
		{
			fullSprite.path = "Game/worldSprites/hare/move/left/";
			break;
		}
		}
		fullSprite.path += std::to_string(currentSprite);
		fullSprite.path += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/hare/stand/down/";
		fullSprite.path += std::to_string(currentSprite);
		fullSprite.path += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/hare/stand/down/1.png";
		currentSprite = 0;
	}
	}

	if (currentAction == move)
	{
		animationLength = 5;
		switch (direction)
		{
		case LEFT:
			fullSprite.path = "Game/worldSprites/hare/move/left/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case RIGHT:
			fullSprite.path = "Game/worldSprites/hare/move/right/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case UP:
			fullSprite.path = "Game/worldSprites/hare/move/up/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case DOWN:
			fullSprite.path = "Game/worldSprites/hare/move/down/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case UPLEFT:
			fullSprite.path = "Game/worldSprites/hare/move/left/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case UPRIGHT:
			fullSprite.path = "Game/worldSprites/hare/move/right/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case DOWNLEFT:
			fullSprite.path = "Game/worldSprites/hare/move/left/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		case DOWNRIGHT:
			fullSprite.path = "Game/worldSprites/hare/move/right/";
			fullSprite.path += std::to_string(currentSprite);
			fullSprite.path += ".png";
			break;
		default:;
		}
	}

	additionalSprites.push_back(fullSprite);

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
}