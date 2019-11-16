#include "Owl.h"
#include "Fern.h"
#include "ForestTree.h"

using namespace sf;

Owl::Owl(const std::string objectName, Vector2f centerPosition) : NeutralMob(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(280, 200);
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	defaultSpeed = 0.0006f;
	speed = 0.0006f;
	animationSpeed = 0.0008f;
	animationLength = 8;
	radius = 70;
	strength = 10;
	sightRange = Helper::GetScreenSize().y * 1 / 2;
	healthPoint = 50;
	currentAction = relax;
	timeAfterHitself = 100000;
	timeForNewHitself = timeAfterHitself;
	timeForNewHit = 1000000;
	toSaveName = "owl";
	tag = Tag::owl;
	canCrashIntoStatic = false;
	zCoord = 10;
}

Owl::~Owl()
{
}

Vector2i Owl::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Owl::setTarget(DynamicObject& object)
{	
	if (object.tag == Tag::noose || currentAction == absorbs)
		return;
	if (Helper::getDist(position, object.getPosition()) <= sightRange)
	{
		if (object.tag == Tag::hero1)
		{
			boundTarget = &object;
			distanceToNearest = Helper::getDist(position, object.getPosition());
		}
	}
}

void Owl::behaviorWithStatic(WorldObject* target, float elapsedTime)
{
	if (currentAction == absorbs)
		return;

	if (target->tag == Tag::fern && Helper::getDist(position, target->getPosition()) <= sightRange * 2 && timeAfterFear >= fearTime)
	{
		if (!(boundTarget && boundTarget->tag == Tag::hero1 && (Helper::getDist(position, boundTarget->getPosition()) <= sightRange ||
			Helper::getDist(target->getPosition(), boundTarget->getPosition()) <= sightRange)))
		{
			boundTarget = target;
			distanceToNearest = Helper::getDist(position, target->getPosition());
		}
	}
	else
	if (target->tag == Tag::tree)
	{
		auto nearestTreeCasted = dynamic_cast<ForestTree*>(target);
		if (nearestTreeCasted && !(boundTarget && boundTarget->tag == Tag::fern))
			if (boundTarget != nullptr && boundTarget->tag == Tag::hero1 && Helper::getDist(nearestTreeCasted->getOwlBase(), position) < Helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) &&
				Helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) > Helper::getDist(position, boundTarget->getPosition()))
				if ((nearestTree != nullptr && Helper::getDist(nearestTreeCasted->getOwlBase(), position) < Helper::getDist(nearestTree->getPosition(), position)) || nearestTree == nullptr)
					if (Helper::getDist(nearestTreeCasted->getOwlBase(), boundTarget->getPosition()) > sightRange * 1.2)
						nearestTree = target;							
	}
}

void Owl::behavior(float elapsedTime)
{
	endingPreviousAction();
	fightLogic(elapsedTime);
	if (healthPoint <= 0)
	{
		changeAction(dead, true);
		direction = STAND;
		return;
	}

	// first-priority actions
	if (boundTarget && boundTarget->tag == Tag::hero1 && Helper::getDist(position, boundTarget->getPosition()) <= sightRange)
			timeAfterFear = 0;	
	else
		timeAfterFear += elapsedTime;

	if (currentAction == absorbs)
	{
		movePosition = position;
		return;
	}
	//-----------------------	

	if (boundTarget == nullptr || boundTarget && boundTarget->tag == Tag::hero1 && Helper::getDist(position, boundTarget->getPosition()) > sightRange)
	{
		auto nearestTreeCasted = dynamic_cast<ForestTree*>(nearestTree);
		if (nearestTree && Helper::getDist(position, nearestTreeCasted->getOwlBase()) > radius)
			{				
				changeAction(move, false, true);
				movePosition = nearestTreeCasted->getOwlBase();
				side = calculateSide(movePosition, elapsedTime);
				return;
			}

		changeAction(relax, true, true);
		direction = STAND;
		movePosition = position;
		return;
	}

	// bouncing to a trap
	if (boundTarget && boundTarget->tag == Tag::fern)
	{
		side = calculateSide(boundTarget->getPosition(), elapsedTime);
		if (Helper::getDist(position, boundTarget->getPosition()) <= radius)
		{
			const auto trap = dynamic_cast<Fern*>(boundTarget);
			if (side == right)
				mirrored = true;
			else
				mirrored = false;
			position = trap->getEnterPosition();
			changeAction(absorbs, true, false);
			movePosition = position;
		}
		else
		{
			changeAction(move, false, true);
			movePosition = boundTarget->getPosition();
		}
	}
	//-------------------

	// runaway from enemy
	if (boundTarget && boundTarget->tag == Tag::hero1)
	{
		float distanceToTarget = Helper::getDist(this->position, boundTarget->getPosition());
		side = calculateSide(movePosition, elapsedTime);
		speed = std::max(defaultSpeed, (defaultSpeed * 10) * (1 - (Helper::getDist(position, boundTarget->getPosition()) / sightRange * 1.5f)));
		animationSpeed = std::max(0.0008f, 0.0008f * speed / defaultSpeed);
		if (distanceToTarget <= sightRange)
		{
			changeAction(move, false, true);
			float k = (sightRange * 1.3f / (sqrt(pow(boundTarget->getPosition().x - position.x, 2) + pow(boundTarget->getPosition().y - position.y, 2))));
			movePosition = Vector2f(position.x + (position.x - boundTarget->getPosition().x) * k, position.y + (position.y - boundTarget->getPosition().y) * k);
		}
	}	
	//-------------------

	if (currentAction != absorbs)
	{
		distanceToNearest = 10e6;
		boundTarget = nullptr;
		nearestTree = nullptr;
	}
}

Vector2f Owl::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Owl::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Owl::endingPreviousAction()
{
	if (lastAction == commonHit)
		currentAction = relax;
	if (lastAction == absorbs)
	{
		auto trap = dynamic_cast<Fern*>(boundTarget);
		trap->inventory.emplace_back(Tag::owl, 1);
		deletePromiseOn();
	}
	lastAction = relax;
}

void Owl::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	return;
}

void Owl::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement fullSprite;

	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();

	switch (currentAction)
	{
	case relax:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/owl/stand/" + DynamicObject::sideToString(side) + '/' + std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case absorbs:
	{		
		animationLength = 7;
		fullSprite.path = "Game/worldSprites/owl/fern/" + std::to_string(currentSprite[0]) + ".png";
		break;
	}
	case dead:
	{
		animationLength = 1;
		fullSprite.path = "Game/worldSprites/owl/stand/down/1.png";
		currentSprite[0] = 1;
		break;
	}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 7;
		fullSprite.path = "Game/worldSprites/owl/move/" + DynamicObject::sideToString(side) + '/' + std::to_string(currentSprite[0]) + ".png";
	}

	additionalSprites.push_back(fullSprite);

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
