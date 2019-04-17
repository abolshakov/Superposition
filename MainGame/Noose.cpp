#include "Noose.h"
#include "Deer.h"

using namespace sf;

Noose::Noose(const std::string objectName, Vector2f centerPosition, WorldObject* owner) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i (360, 300);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	defaultSpeed = 0.0007f;
	speed = defaultSpeed;
	animationSpeed = 5e-4;
	animationLength = 8;
	radius = 0;
	currentAction = move;
	routeGenerationAbility = false;
	jerk(2, 1);
	toSaveName = "noose";
	tag = nooseTag;
}

Noose::~Noose()
{
}

Vector2i Noose::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	ropeElongation = textureBox.width / 15;
	return Vector2i(0, textureBox.height / 1.8);	
}

void Noose::setTarget(DynamicObject& object)
{
	return;
}

void Noose::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	if (target->getId() == deer)
	{
		auto deer = dynamic_cast<Deer*>(target);
		if (Helper::getDist(position, deer->getHeadPosition()) <= radius + target->getRadius())
		{
			boundTarget = target;
			if (deer->getOwner() == nullptr && deer->getCurrentAction() != commonHit)
				deer->changeAction(commonHit, true, false);
			deer->setOwner(owner);
		}
	}

    return;
}

void Noose::behaviorWithStatic(WorldObject* target, float elapsedTime)
{

}

void Noose::behavior(float elapsedTime)
{
	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (boundTarget != nullptr)
	{
		auto deer = dynamic_cast<Deer*>(boundTarget);
		position = deer->getHeadPosition();		
		speed = 0;
		changeAction(relax, false, false);
	}
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

	if (forgetSelectedTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}
}

void Noose::endingPreviousAction()
{
	if (lastAction == jerking)
	{
		currentAction = dead;
		stillLoop = additionalSprites[0];
		stillRope = additionalSprites[1];
        //delatePromiseOn();
	}
	lastAction = relax;
}

void Noose::jerkInteract(float elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = (jerkDistance / jerkDuration) * jerkPower * pow(jerkTime / jerkDuration, jerkDeceleration);
			speed = std::max(defaultSpeed / jerkDeceleration, speed);
		}
		else
			speed = 0;
	}
}

void Noose::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 40 / animationSpeed * 13;
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	currentSprite[0] = 1;

	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);
	const float coeff = jerkDistance / Helper::getDist(mousePos, screenCenter);
	movePosition = Vector2f(owner->getPosition().x + (mousePos.x - screenCenter.x) * coeff, owner->getPosition().y + (mousePos.y - screenCenter.y) * coeff);
}

void Noose::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement ropeSprite, loopSprite;

	ropeSprite.offset = Vector2f(0, 15);
	ropeSprite.size = Vector2f(this->conditionalSizeUnits);
    loopSprite.offset = Vector2f(this->textureBoxOffset);
	loopSprite.size = Vector2f(this->conditionalSizeUnits);
	ropeSprite.size.y = 30;

	additionalSprites.clear();

	switch (currentAction)
	{
		case relax:
		{
			animationLength = 13;
			animationSpeed = 0.0005f;
			ropeSprite.path = "Game/worldSprites/noose/rope/caught.png";
			ropeSprite.size.y = 60;
			if (owner->getPosition().x < position.x)
				ropeSprite.size.y *= -1;
			ropeSprite.offset = Vector2f(0, 0);
            loopSprite.path = "Game/worldSprites/noose/rope/knot.png";
			loopSprite.size = Vector2f(40, 30);
			loopSprite.offset = Vector2f(float(loopSprite.size.x)*getScaleRatio().x / 1.8, float(loopSprite.size.y)*getScaleRatio().y / 1.8);
			break;
		}
		case jerking:
		{
			animationLength = 13;
			animationSpeed = 0.0005f;			
			ropeSprite.path = "Game/worldSprites/noose/rope/thrown.png";
			loopSprite.path = "Game/worldSprites/noose/loop/";			
			loopSprite.path += std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			additionalSprites.push_back(stillRope);
			additionalSprites.push_back(stillLoop);
			break;
		}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 13;
		animationSpeed = 0.0005f;
		ropeSprite.path = "Game/worldSprites/noose/nooseRope.png";
		loopSprite.path = "Game/worldSprites/noose/noose/loop/" + std::to_string(currentSprite[0]) + ".png";
	}

	if (owner != nullptr)
	{
		Vector2f ownerPos = Vector2f(owner->getPosition().x, owner->getPosition().y);
		ropeSprite.size = Vector2f(sqrt(pow(owner->getPosition().x - position.x, 2) + pow(owner->getPosition().y - position.y, 2)) + ropeElongation, ropeSprite.size.y); // a little bit longer rope for sprite joining		
		if (position.y <= owner->getPosition().y)
			ropeSprite.rotation = acos((owner->getPosition().x - position.x) / sqrt(pow(owner->getPosition().x - position.x, 2) + pow(owner->getPosition().y - position.y, 2))) / pi * 180;
		else
			ropeSprite.rotation = -acos((owner->getPosition().x - position.x) / sqrt(pow(owner->getPosition().x - position.x, 2) + pow(owner->getPosition().y - position.y, 2))) / pi * 180;
	}

	ropeSprite.offset.x += sin(ropeSprite.rotation / 180 * pi) * (ropeSprite.size.y - ropeSprite.offset.y);
	ropeSprite.offset.y += (1 - cos(ropeSprite.rotation / 180 * pi)) * (ropeSprite.size.y - ropeSprite.offset.y);

	if (currentAction != relax)
	{
		loopSprite.rotation = ropeSprite.rotation + 180;
		loopSprite.offset.x += sin(loopSprite.rotation / 180 * pi) * (loopSprite.size.y - loopSprite.offset.y); // rotational position correction
		loopSprite.offset.y += (1 - cos(loopSprite.rotation / 180 * pi)) * (loopSprite.size.y - loopSprite.offset.y);
		ropeSprite.offset.x += (owner->getPosition().x - position.x) * ropeElongation / Helper::getDist(position, owner->getPosition()); // offset of the extended rope
		ropeSprite.offset.y += (owner->getPosition().y - position.y) * ropeElongation / Helper::getDist(position, owner->getPosition());
	}

	additionalSprites.push_back(ropeSprite);
    additionalSprites.push_back(loopSprite);

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