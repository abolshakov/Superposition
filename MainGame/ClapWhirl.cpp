#include "ClapWhirl.h"
#include "Deerchant.h"
#include "Nightmare3.h"

using namespace sf;

ClapWhirl::ClapWhirl(const std::string objectName, Vector2f centerPosition, WorldObject* owner) : DynamicObject(objectName, centerPosition)
{
	conditionalSizeUnits = Vector2i(250, 500);
	startPos = owner->getPosition();
	currentSprite[0] = 1;
	timeForNewSprite = 0;
	this->owner = owner;
	defaultSpeed = 0.00055f;	
	strength = 15;
	animationSpeed = 10;
	animationLength = 8;
	radius = 120;
	currentAction = move;
	routeGenerationAbility = false;
	canCrashIntoDynamic = false;
	jerk(2, 1);
	toSaveName = "clapWhirl";
	tag = Tag::clapWhirl;
}

ClapWhirl::~ClapWhirl()
{
}

Vector2i ClapWhirl::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(0, int(textureBox.height / 2));
}

void ClapWhirl::setTarget(DynamicObject& object)
{
	return;
}

void ClapWhirl::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	if (currentAction == dead)
		return;	

	if (target->tag == Tag::hero1)
	{		
		if (Helper::getDist(position, target->getPosition()) <= radius + target->getRadius() && color.a >= 40)
		{
			target->takeDamage(strength, position);
			deletePromiseOn();
		}
	}

	return;
}

void ClapWhirl::behaviorWithStatic(WorldObject* target, float elapsedTime)
{
	if (currentAction == dead)
		return;
	/*const auto terrain = dynamic_cast<TerrainObject*>(target);
	if (terrain && Helper::getDist(position, target->getPosition()) <= radius + target->getRadius() && cantStopMe.count(target->tag) == 0)	
		currentAction = dead;*/
}

void ClapWhirl::behavior(float elapsedTime)
{
	if (currentAction == dead)
		stopping(true, true);

	endingPreviousAction();
	jerkInteract(elapsedTime);

	if (owner != nullptr)
	{
		ownerPos = Vector2f(owner->getPosition().x + 0, owner->getPosition().y - 0);
		ownerGlobalBounds = owner->getConditionalSizeUnits();
	}

	if (jerkTime / jerkDuration <= 1)
		color.a = 255 * jerkTime / jerkDuration;
	else
		color.a = 0;
}	

Vector2f ClapWhirl::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int ClapWhirl::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void ClapWhirl::stopping(bool doStand, bool forgetSelectedTarget)
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

void ClapWhirl::endingPreviousAction()
{
	if (lastAction == jerking)
	{
		currentAction = dead;
		deletePromiseOn();
	}
	if (lastAction == dead)
		deletePromiseOn();
	lastAction = relax;
}

void ClapWhirl::jerkInteract(float elapsedTime)
{
	if (currentAction == jerking)
	{
		if (jerkTime > 0)
		{
			jerkTime -= elapsedTime;
			speed = Helper::getDist(startPos, movePosition) / jerkDuration;
		}
		else		
			stopping(true, true);		
	}
}

void ClapWhirl::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	stopping(false, false);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 7e5;
	this->jerkTime = this->jerkDuration;
	currentAction = jerking;
	jerkDistance = 1400;
	currentSprite[0] = 1;

	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);	

	auto nightmare = dynamic_cast<Nightmare3*>(owner);
	if (nightmare)
	{
		Vector2f flightVector = Vector2f(nightmare->getVictim()->getPosition().x - nightmare->getPosition().x,
			nightmare->getVictim()->getPosition().y - nightmare->getPosition().y);
		float k = jerkDistance / sqrt(pow(flightVector.x, 2) + pow(flightVector.y, 2));
		movePosition = Vector2f(owner->getPosition().x + flightVector.x * k, owner->getPosition().y + flightVector.y * k);
	}
	else
		movePosition = { 0, 0 };
}

void ClapWhirl::fightLogic(float elapsedTime, DynamicObject* target)
{
	return;
}

void ClapWhirl::rotateAndExtend(spriteChainElement* whirl)
{
	if (ownerPos != Vector2f(0, 0))
	{
		if (position.y <= startPos.y)
			whirl->rotation = acos((startPos.x - position.x) / sqrt(pow(startPos.x - position.x, 2) + pow(startPos.y - position.y, 2))) / pi * 180;
		else
			whirl->rotation = -acos((startPos.x - position.x) / sqrt(pow(startPos.x - position.x, 2) + pow(startPos.y - position.y, 2))) / pi * 180;
	}

	if (currentAction != relax)
	{
		whirl->rotation = whirl->rotation + 180;
		whirl->offset.x -= sin(whirl->rotation / 180 * pi) * (textureBoxOffset.y); // rotational position correction
		whirl->offset.y -= (1 - cos(whirl->rotation / 180 * pi)) * (textureBoxOffset.y);
	}
}

void ClapWhirl::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement whirlSprite;

	whirlSprite.offset = Vector2f(this->textureBoxOffset);
	whirlSprite.size = Vector2f(this->conditionalSizeUnits);
	additionalSprites.clear();
	animationSpeed = 10;

	switch (currentAction)
	{
	case relax:
	{
		animationLength = 13;
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
		deletePromiseOn();
		break;
	}
	case jerking:
	{
		animationLength = 13;
		//whirlSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
		break;
	}
	case dead:
	{
		animationLength = 5;		
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirlBroken.png";
		break;
	}
	default:;
	}

	if (currentAction == move)
	{
		animationLength = 13;	
		//whirlSprite.path = "Game/worldSprites/noose/nooseLoop/" + std::to_string(currentSprite[0]) + ".png";
		whirlSprite.path = "Game/worldSprites/nightmare3/clap/whirl.png";
	}

	rotateAndExtend(&whirlSprite);
	additionalSprites.push_back(whirlSprite);
	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1e6 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
	}
}