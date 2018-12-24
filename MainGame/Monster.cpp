#include "Monster.h"

using namespace sf;

Monster::Monster(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (200, 250);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0004f;
	animationSpeed = 0.0005f;
	animationLength = 8;
	radius = 100;
	strength = 10;
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

	setSide(Vector2i(movePosition), elapsedTime);

	srand(time(0));
	Vector2i curPos = Vector2i(this->getPosition());
	Vector2i tarPos = Vector2i(target.getPosition());

	if (sqrt(pow(curPos.x - tarPos.x, 2) + pow(curPos.y - tarPos.y, 2)) <= (this->radius + target.getRadius()))
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
				if ((target.getCurrentAction() != evasionDown && currentAction != upperHit) || (target.getCurrentAction() != evasionUp && currentAction != bottomHit))
				{
					target.takeDamage(10);
					wasHit = true;
				}
			}
		}
		direction = STAND;
		if (currentAction == combatState)
		{
			wasHit = false;
			timeAfterHit += timer.getElapsedTime().asMilliseconds();
			if (timeAfterHit >= timeForNewHit)
			{
				//currentAction = commonHit;
				currentAction = bottomHit;
				currentSprite = 0;
				timeAfterHit = 0;
				timer.restart();
			}
		}
	}
	else
	{
		if (currentAction != upperHit && currentAction != bottomHit && currentAction != directHit)
		{
			timeAfterHit = 0;
			timer.restart();
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

std::string Monster::getSpriteName(long long elapsedTime)
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
	case upperHit:
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
	case directHit:
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
		switch (side)
		{
		case left:
			spriteName = "Game/worldSprites/hare/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case right:
			spriteName = "Game/worldSprites/hare/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case up:
			spriteName = "Game/worldSprites/hare/move/up/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case down:
			spriteName = "Game/worldSprites/hare/move/down/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		/*case UPLEFT:
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
			break;*/
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