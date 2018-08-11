#include "Monster.h"

using namespace sf;

Monster::Monster(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2f(200, 250);
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0002f;
	animationSpeed = 0.0005f;
	animationLength = 8;
	radius = 70;
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
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
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

	setSide(target.getPosition());

	srand(time(0));
	Vector2f curPos = this->getPosition();
	Vector2f tarPos = target.getPosition();	

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
			targetPosition = target.getPosition();
			moveToTarget(target.getRadius());
		}
	}
}

std::string Monster::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
		case bottomHit:
		{
			animationLength = 8;
			switch (side)
			{
			case up:
			{
				spriteName = "m1/m1_fight/m1_fight_b/m1_fight_b_";
				break;
			}
			case right:
			{
				spriteName = "m1/m1_fight/m1_fight_r/m1_fight_r_";
				break;
			}
			case down:
			{
				spriteName = "m1/m1_fight/m1_fight_f/m1_fight_f_";
				break;
			}
			case left:
			{
				spriteName = "m1/m1_fight/m1_fight_l/m1_fight_l_";
				break;
			}
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		}
		case upperHit:
		{
			animationLength = 8;
			switch (side)
			{
			case up:
			{
				spriteName = "m1/m1_fight/m1_fight_b/m1_fight_b_";
				break;
			}
			case right:
			{
				spriteName = "m1/m1_fight/m1_fight_r/m1_fight_r_";
				break;
			}
			case down:
			{
				spriteName = "m1/m1_fight/m1_fight_f/m1_fight_f_";
				break;
			}
			case left:
			{
				spriteName = "m1/m1_fight/m1_fight_l/m1_fight_l_";
				break;
			}
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		}
		case directHit:
		{
			animationLength = 8;
			switch (side)
			{
			case up:
			{
				spriteName = "m1/m1_fight/m1_fight_b/m1_fight_b_";
				break;
			}
			case right:
			{
				spriteName = "m1/m1_fight/m1_fight_r/m1_fight_r_";
				break;
			}
			case down:
			{
				spriteName = "m1/m1_fight/m1_fight_f/m1_fight_f_";
				break;
			}
			case left:
			{
				spriteName = "m1/m1_fight/m1_fight_l/m1_fight_l_";
				break;
			}
			}
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		}
		case combatState:
		{
			animationLength = 8;
			switch (side)
			{
			case up:
			{
				spriteName = "m1/m1_still/m1_still_b";
				break;
			}
			case right:
			{
				spriteName = "m1/m1_still/m1_still_r";
				break;
			}
			case down:
			{
				spriteName = "m1/m1_still/m1_still_f";
				break;
			}
			case left:
			{
				spriteName = "m1/m1_still/m1_still_l";
				break;
			}
			}
			//spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		}
		case relax:
		{
			animationLength = 8;
			switch (side)
			{
			case up:
			{
				spriteName = "m1/m1_still/m1_still_b";
				break;
			}
			case right:
			{
				spriteName = "m1/m1_still/m1_still_r";
				break;
			}
			case down:
			{
				spriteName = "m1/m1_still/m1_still_f";
				break;
			}
			case left:
			{
				spriteName = "m1/m1_still/m1_still_l";
				break;
			}
			}
			//spriteName += std::to_string(currentSprite);
			//currentSprite = 0;
			spriteName += ".png";
			break;
		}
		case dead:
		{
			animationLength = 8;
			spriteName = "m1/m1_still/m1_still_f.png";
			currentSprite = 0;
		}
	}

	if (currentAction == move)
	{
		animationLength = 8;
		switch (direction)
		{
		case LEFT:
			spriteName = "m1/m1_run/m1_run_l/m1_run_l_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case RIGHT:
			spriteName = "m1/m1_run/m1_run_r/m1_run_r_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "m1/m1_run/m1_run_b/m1_run_b_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "m1/m1_run/m1_run_f/m1_run_f_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "m1/m1_run/m1_run_l/m1_run_l_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPRIGHT:
			spriteName = "m1/m1_run/m1_run_r/m1_run_r_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNLEFT:
			spriteName = "m1/m1_run/m1_run_l/m1_run_l_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNRIGHT:
			spriteName = "m1/m1_run/m1_run_r/m1_run_r_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
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