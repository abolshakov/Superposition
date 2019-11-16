#include "OwlBoss.h"

using namespace sf;

OwlBoss::OwlBoss(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(600, 600);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	defaultSpeed = 0.0005f;
	animationSpeed = 0.0005f;
	animationLength = 1;
	currentSprite[0] = 1;
	radius = 100;
	strength = 100;
	healthPoint = 1000;
	currentAction = rightFlap;
	timeAfterHitself = 100000;
	timeForNewHitself = timeAfterHitself;
	timeForNewHit = 2000000;
	timeForNewRoute = 2000000;
	routeGenerationAbility = false;
	canCrashIntoDynamic = false;

	toSaveName = "monster";
}

OwlBoss::~OwlBoss()
{

}

void OwlBoss::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void OwlBoss::behavior(float elapsedTime)
{
	fightLogic(elapsedTime);
	//jerk interaction
	if (isJerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = (jerkDistance / jerkDuration) * pow(jerkTime / jerkDuration, jerkDeceleration);
			speed = std::max(defaultSpeed / jerkDeceleration, speed);
			//speed = defaultSpeed;
		}
		else
		{
			isJerking = false;
			speed = defaultSpeed;
		}
	}
	//----------------

	//processing ending of previous actions
	if (lastAction == upFlap || lastAction == leftFlap || lastAction == rightFlap || lastAction == startFlap)
	{
		if (--flapsBeforeJerkCount > 0)
			if (flapsBeforeJerkCount == 1)
				changeAction(startFlap, true, false);
			else
				changeAction(Actions(30 + rand() % 3), true, false);
		lastAction = relax;
		//changeAction(rightFlap, false, true);
		//lastAction = relax;
	}

	//------------------

	//processing current actions
	if (currentAction == rightFlap && !isJerking && currentSprite[0] == 7)
		jerk(2, 1, Vector2f(position.x - 200, position.y));
	if (currentAction == leftFlap && !isJerking && currentSprite[0] == 7)
		jerk(2, 1, Vector2f(position.x + 200, position.y));
	//--------------------------
}

Vector2i OwlBoss::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void OwlBoss::setTarget(DynamicObject& object)
{
	if (object.tag == Tag::hero1)
		return; //targetPosition = object.getPosition();
}

void OwlBoss::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	debugInfo = Helper::getDist(this->position, movePosition) / jerkDistance * 1000;
	if (healthPoint <= 0)
	{
		changeAction(dead, true, false);
		direction = STAND;
		return;
	}

	if (target->tag != Tag::hero1)
		return;

	side = calculateSide(movePosition, elapsedTime);

	timeAfterHit += elapsedTime;
	timeAfterNewRoute += elapsedTime;

	if (jerkDistance != 0 && currentAction == move)
	{
		float dragCoefficient = 1.2f; // 1..2
		float startPower = 1.1f; //1..2
		this->speed = pow(this->defaultSpeed, dragCoefficient / (startPower + (timeAfterHit / timeForNewHit)));
		if (Helper::getDist(this->position, target->getPosition()) / jerkDistance <= 0.2f)
			this->speed *= Helper::getDist(this->position, target->getPosition()) / jerkDistance;
	}
	this->speed = std::max(defaultSpeed / 10, speed);

	if (flapsBeforeJerkCount > 1)
	{
		timeAfterHit = 0;
		if (!isJerking)
			movePosition = this->position;
		return;
	}
	if (flapsBeforeJerkCount == 0)
	{
		changeAction(move, false, true);
		this->speed = defaultSpeed;
		jerkDistance = Helper::getDist(this->position, target->getPosition());
	}

	if (speed <= defaultSpeed * 3) //with greater acceleration the owl loses the ability to coordinate the route
	{
		movePosition = Vector2f(target->getPosition().x + (target->getPosition().x - this->position.x) / 1.0f, target->getPosition().y + (target->getPosition().y - this->position.y) / 1.0f);
	}

	if (Helper::getDist(this->position, movePosition) / speed <= (40 / animationSpeed) * 3 && currentAction == move)
	{
		changeAction(stopFlap, true, false);
	}

	if (Helper::getDist(this->position, movePosition) <= this->radius) //hit interaction
	{
		if (Helper::getDist(this->position, target->getPosition()) <= this->radius)
			target->takeDamage(this->strength, position);
		timeAfterHit = 0;
		timeAfterNewRoute = 0;
		flapsBeforeJerkCount = rand() % 3 + 3;
		changeAction(upFlap, true, true);
		this->speed = defaultSpeed;
	}
	//-----------------
}

Vector2f OwlBoss::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int OwlBoss::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void OwlBoss::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 40 / animationSpeed * 10;
	this->jerkTime = this->jerkDuration;
	isJerking = true;
	jerkDistance = 400;

	movePosition = Vector2f(destinationPoint);
}

void OwlBoss::fightLogic(float elapsedTime, DynamicObject* target)
{
	pushAway(elapsedTime);
}

void OwlBoss::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	std::string spriteName;

	spriteChainElement legsSprite, bodySprite, fullSprite;
	additionalSprites.clear();
	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2f(this->conditionalSizeUnits);
	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);

	switch (currentAction)
	{
	case upFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/upFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case leftFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/leftFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case rightFlap:
	{
		animationLength = 10;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/flap/rightFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case startFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/startFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case stopFlap:
	{
		animationLength = 3;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/stopFlap/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case relax:
	{
		animationLength = 1;
		animationSpeed = 0.0005f;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		bodySprite.path = "Game/worldSprites/owlBoss/stand/down/1.png";
		currentSprite[0] = 1;
	}
	}

	if (currentAction == move)
	{
		animationLength = 12;
		animationSpeed = 0.0005f;
		if (movePosition.y >= this->position.y)
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/down/";
		else
			bodySprite.path = "Game/worldSprites/owlBoss/attack1/jerk/up/";
		bodySprite.path += std::to_string(currentSprite[0]);
		bodySprite.path += ".png";
	}

	additionalSprites.push_back(bodySprite);

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