#include "Noose.h"
#include "Deer.h"
#include "Deerchant.h"

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
	radius = 50;
	currentAction = move;
	routeGenerationAbility = false;
	canCrashIntoDynamic = false;
	jerk(2, 1);
	toSaveName = "noose";
	tag = Tag::noose;
}

Noose::~Noose()
{
}

Vector2i Noose::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);	
	ropeElongation = textureBox.width / 20.0f;
	//ropeElongation = 0;
	return Vector2i(0, int(textureBox.height / 1.8));
}

void Noose::setTarget(DynamicObject& object)
{
	return;
}

void Noose::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	if (currentAction == dead)
		return;

	if (target->tag == Tag::deer)
	{		
		auto deer = dynamic_cast<Deer*>(target);		

		if (Helper::getDist(position, deer->getHeadPosition()) <= radius + target->getRadius())
		{
			boundTarget = target;
			if (deer->getOwner() == nullptr && deer->getCurrentAction() != commonHit)
				deer->changeAction(commonHit, true, false);
			if (owner != nullptr)
				deer->setOwner(owner);
		}
	}

    return;
}

void Noose::behaviorWithStatic(WorldObject* target, float elapsedTime)
{
	if (currentAction == dead)
		return;
}

void Noose::behavior(float elapsedTime)
{
	if (currentAction == dead)
		return;

	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (owner != nullptr)
	{
		ownerPos = Vector2f(owner->getPosition().x + owner->getConditionalSizeUnits().x / 10.0f, owner->getPosition().y - owner->getConditionalSizeUnits().y / 13.0f);
		ownerGlobalBounds = owner->getConditionalSizeUnits();
	}

	if (boundTarget != nullptr && owner != nullptr)
		if (Helper::getDist(position, owner->getPosition()) >= maximumLength)
		{
			auto deer = dynamic_cast<Deer*>(boundTarget);
			if (deer)
				deer->setOwner(nullptr);
			stopping(true, true);			
			changeAction(dead, true, false);			
			owner = nullptr;		
		}

	if (boundTarget != nullptr)
	{
		auto deer = dynamic_cast<Deer*>(boundTarget);
		position = deer->getHeadPosition();
		speed = 0;
		changeAction(relax, false, true);
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
		stillRope = additionalSprites[0];
		stillLoop = additionalSprites[1];
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

void Noose::fightLogic(float elapsedTime, DynamicObject* target)
{
	return;
}

void Noose::rotateAndExtend(spriteChainElement* rope, spriteChainElement* loop)
{
	if (ownerPos != Vector2f(0, 0))
	{
		const Vector2f beginPoint = Vector2f(ownerPos.x/* + ownerGlobalBounds.x / 12.0f*/, ownerPos.y - ownerGlobalBounds.y / 13.0f);
		rope->size = Vector2f(Helper::getDist(beginPoint, position) + ropeElongation, rope->size.y); // a little bit longer rope for sprite joining		
		if (position.y <= beginPoint.y)
			rope->rotation = acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
		else
			rope->rotation = -acos((beginPoint.x - position.x) / sqrt(pow(beginPoint.x - position.x, 2) + pow(beginPoint.y - position.y, 2))) / pi * 180;
	}

	if (currentAction != relax)
	{
		loop->rotation = rope->rotation + 180;
		loop->offset.x -= sin(loop->rotation / 180 * pi) * (textureBoxOffset.y); // rotational position correction
		loop->offset.y -= (1 - cos(loop->rotation / 180 * pi)) * (textureBoxOffset.y);
	}

	if (ownerPos != Vector2f(0, 0))
	{
		rope->offset.x += (ownerPos.x - position.x) * ropeElongation / Helper::getDist(position, ownerPos); // offset of the extended rope
		rope->offset.y += (ownerPos.y - position.y) * ropeElongation / Helper::getDist(position, ownerPos);

		// change position to hero belt
		auto dynOwner = dynamic_cast<Deerchant*>(owner);
		if (dynOwner)
		{
			rope->position = dynOwner->getBeltPosition();
			rope->offset = Vector2f(rope->offset.x + rope->position.x - position.x, rope->offset.y + rope->position.y - position.y);
		}
	}
}

void Noose::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement ropeSprite, loopSprite;

	ropeSprite.offset = Vector2f(0, 0);
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
			ropeSprite.path = "Game/worldSprites/noose/nooseRope/caught.png";
			ropeSprite.size.y = 60;
			if (owner != nullptr)
				if (owner->getPosition().x < position.x)
					ropeSprite.size.y *= -1;
            loopSprite.path = "Game/worldSprites/noose/nooseRope/knot.png";
			loopSprite.size = Vector2f(40, 30);
			loopSprite.offset = Vector2f(float(loopSprite.size.x)*getScaleRatio().x / 1.8, float(loopSprite.size.y)*getScaleRatio().y / 1.8);
			break;
		}
		case jerking:
		{
			animationLength = 13;
			animationSpeed = 0.0005f;			
			ropeSprite.path = "Game/worldSprites/noose/nooseRope/thrown.png";
			loopSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
			break;
		}
		case dead:
		{
			animationLength = 1;
			animationSpeed = 0.0005f;					

			ropeSprite.path = "Game/worldSprites/noose/nooseRope/thrown.png";
			loopSprite.path = "Game/worldSprites/noose/nooseLoop/1.png";

			rotateAndExtend(&ropeSprite, &loopSprite);

			additionalSprites.push_back(ropeSprite);
			additionalSprites.push_back(loopSprite);
			return;
		}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 13;
		animationSpeed = 0.0005f;
		ropeSprite.path = "Game/worldSprites/noose/nooseRope/thrown.png";
		loopSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
	}

	rotateAndExtend(&ropeSprite, &loopSprite);

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