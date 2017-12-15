#include "Enemy.h"
#include "Helper.h"

using namespace sf;

Enemy::Enemy(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2f(200, 400);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0004f;
	animationLength = 8;
	radius = 50;
	strength = 10;
	healthPoint = 100;
	currentAction = relax;
	timeAfterHitself = 100000;
	timeForNewHitself = timeAfterHitself;
}

Vector2i Enemy::calculateTextureOffset()
{	
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Enemy::behavior(DynamicObject& target)
{
	if (healthPoint <= 0)
	{
		currentAction = dead;
		direction = STAND;
	}
	else
	{
		srand(time(0));
		Vector2f curPos = this->getPosition();
		Vector2f tarPos = target.getPosition();
		this->targetPosition = target.getPosition();

		if (sqrt(pow(curPos.x - tarPos.x, 2) + pow(curPos.y - tarPos.y, 2)) <= (this->radius + target.radius))
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
					if ((target.currentAction != evasionDown && currentAction != upperHit) || (target.currentAction != evasionUp && currentAction != bottomHit))
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
					while (true)
					{
						currentAction = (Actions)(rand() % 3);
						if (currentAction != lastAction)
							break;
					}
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
				moveToTarget(tarPos, target.radius);
			}
		}
	}
}

void Enemy::moveToTarget(Vector2f targetPosition, float targetRadius)
{
	Vector2f curPos = this->getPosition();
	Vector2f tarPos = targetPosition;
	if (abs(curPos.x - tarPos.x) < (this->radius + targetRadius))
	{
		if (curPos.y < tarPos.y)
		{
			direction = DOWN;
			return;
		}
		direction = UP;
		return;
	}
	if (abs(curPos.y - tarPos.y) < (this->radius + targetRadius))
	{
		if (curPos.x < tarPos.x)
		{
			direction = RIGHT;
			return;
		}
		direction = LEFT;
		return;
	}
	if (curPos.x < tarPos.x)
	{
		if (curPos.y < tarPos.y)
		{
			direction = DOWNRIGHT;
		}
		else
			if (curPos.y > tarPos.y)
			{
				direction = UPRIGHT;
			}
	}
	else
		if (curPos.x > tarPos.x)
		{
			if (curPos.y < tarPos.y)
			{
				direction = DOWNLEFT;
			}
			else
				if (curPos.y > tarPos.y)
				{
					direction = UPLEFT;
				}
		}
}

std::string Enemy::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

		switch (currentAction)
		{
		case bottomHit:
			animationLength = 8;
			switch (hitDirection)
			{
			case up:
				spriteName = "enemy/enemyL_";
			case right:
				spriteName = "enemy/enemyL_";
			case down:
				spriteName = "enemy/enemyL_";
			case left:
				spriteName = "enemy/enemyL_";
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case upperHit:
			animationLength = 8;
			switch (hitDirection)
			{
			case up:
				spriteName = "enemy/enemyR_";
			case right:
				spriteName = "enemy/enemyR_";
			case down:
				spriteName = "enemy/enemyR_";
			case left:
				spriteName = "enemy/enemyR_";
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case directHit:
			animationLength = 8;
			switch (hitDirection)
			{
			case up:
				spriteName = "enemy/enemyF_";
			case right:
				spriteName = "enemy/enemyF_";
			case down:
				spriteName = "enemy/enemyF_";
			case left:
				spriteName = "enemy/enemyF_";
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case combatState:
			animationLength = 8;
			switch (hitDirection)
			{
			case up:
				spriteName = "enemy/enemyB_";
			case right:
				spriteName = "enemy/enemyB_";
			case down:
				spriteName = "enemy/enemyB_";
			case left:
				spriteName = "enemy/enemyB_";
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case relax:
			animationLength = 1;
			spriteName = "enemy/enemyF_0.png";
			currentSprite = 0;
			break;
		case dead:
			animationLength = 1;
			spriteName = "enemy/enemyF_0.png";
			currentSprite = 0;
		}

	if (currentAction == move)
	{
		animationLength = 8;
		switch (direction)
		{
		case LEFT:
			spriteName = "enemy/enemyL_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case RIGHT:
			spriteName = "enemy/enemyR_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "enemy/enemyB_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "enemy/enemyF_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "enemy/enemyBL_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case UPRIGHT:
			spriteName = "enemy/enemyBR_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case DOWNLEFT:
			spriteName = "enemy/enemyFL_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case DOWNRIGHT:
			spriteName = "enemy/enemyFR_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		default:;
		}
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / speed)
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