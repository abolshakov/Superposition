#include "Deerchant.h"
#include "Helper.h"
#include "PickedObject.h"
#include "Noose.h"
#include "Brazier.h"
#include "WreathTable.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite[0] = 1;
	currentSprite.resize(2);
	timeForNewSprite = 0;
	defaultSpeed = 0.0006f;
	speed = defaultSpeed;
	animationSpeed = 0.0010f;
	animationLength = 8;	
	radius = 50;
	hitDistance = 50;
	strikingSprite = 6;
	strength = 10;
	maxHealthPointValue = 100;
	healthPoint = maxHealthPointValue;
	energy = 50; maxEnergyValue = 100;
	currentAction = relax;
	toSaveName = "this1";
	tag = Tag::hero1;
	//canCrashIntoDynamic = false;

	for (int i = 0; i < 3; i++)
		bags.push_back((*new HeroBag));
	const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
		closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
	bags[0].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y), true);
	bags[1].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, Helper::GetScreenSize().y - closedBagSize.y), true);
	bags[2].initialize(Vector2f(Helper::GetScreenSize().x / 3 - closedBagSize.x, Helper::GetScreenSize().y / 2 - closedBagSize.y), true);
}

Deerchant::~Deerchant()
{

}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2i(375, 375);
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 4 / 5);
}

void Deerchant::handleInput(bool usedMouse)
{	
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == dropping || currentAction == builds || currentAction == jerking)
		return;

	//if (currentAction == throwNoose && heldItem->content != std::make_pair(Tag::noose, 1))
		//changeAction(relax, true, false);

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && direction != STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveOffset = Vector2f(-1, -1);
	direction = STAND;
	if (Keyboard::isKeyPressed(Keyboard::A))					
		direction = LEFT;					
	if (Keyboard::isKeyPressed(Keyboard::D))						
		direction = RIGHT;						
	if (Keyboard::isKeyPressed(Keyboard::W))							
		direction = UP;							
	if (Keyboard::isKeyPressed(Keyboard::S))								
		direction = DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
		direction = UPLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		direction = UPRIGHT;
	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
		direction = DOWNLEFT;
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
		direction = DOWNRIGHT;

	if (currentAction != throwNoose) //second priority actions, interact while moving
	{
		if (!(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)))
		{
			bool isIntersect = false;
			if (boundTarget)
				isIntersect = (sqrt(pow(this->position.x - movePosition.x, 2) + pow(this->position.y - movePosition.y, 2)) <= (this->radius + boundTarget->getRadius()));
			if (isIntersect || !boundTarget)
			{
				direction = STAND;
				if (currentAction == move)
					changeAction(relax, true, false);
			}
			else
			{
				if (boundTarget && Helper::getDist(position, movePosition) > (this->radius + boundTarget->getRadius()))
				{
					setMoveOffset(0);
					changeAction(move, currentAction != move, false);
				}
			}
		}
	}
	else
		return;

	//define actions
	if (Mouse::isButtonPressed(Mouse::Left) && heldItem->content.first == Tag::noose)
	{
		changeAction(throwNoose, true, false);
		return;
	}

	if (direction != STAND && currentAction != moveHit && !Mouse::isButtonPressed(Mouse::Left))
		changeAction(move, currentAction == relax, currentAction != move);

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (boundTarget != nullptr)
			stopping(true, true);
		if (!Mouse::isButtonPressed(Mouse::Left))
			stopping(false, false, true);
	}
	//--------------

	if (isBuildSystemMaker)
		return;
	if (Keyboard::isKeyPressed(Keyboard::Z) && (currentAction == relax))
	{
		changeAction(transitionToEnotherWorld, true, false);
		currentSprite[0] = 1;
	}
	else
		if (Mouse::isButtonPressed(Mouse::Left) && !usedMouse)
		{
			if (direction != STAND)			
				changeAction(moveHit, !(currentAction == moveHit || currentAction == commonHit), false);			
			else
				changeAction(commonHit, !(currentAction == moveHit || currentAction == commonHit), false);
		}
}

void Deerchant::changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
}

void Deerchant::setHitDirection()
{
	auto screenSize = Helper::GetScreenSize();
	float xPos = screenSize.x / 2, yPos = screenSize.y / 2, mouseX = Mouse::getPosition().x, mouseY = Mouse::getPosition().y;
	float alpha = atan((yPos - mouseY) / (xPos - mouseX)) * 180 / pi;
	if (mouseY <= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
		side = up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			side = right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				side = down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					side = left;
}

void Deerchant::setTarget(DynamicObject& object)
{
	nearTheTable = false;
	return;
}

void Deerchant::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{
	const bool isIntersect = Helper::getDist(position, target->getPosition()) <= this->radius + target->getRadius() + hitDistance;

	if (isIntersect && calculateSide(target->getPosition(), elapsedTime) != invertSide(side))
	{
		if (target->timeForNewHitself >= target->getTimeAfterHitself())
		{
			if ((this->currentAction == commonHit || this->currentAction == moveHit) && (this->getSpriteNumber() == 4 || this->getSpriteNumber() == 5 || this->getSpriteNumber() == 8))
			{
				this->addEnergy(5);
				target->takeDamage(this->getStrength(), position);
				target->timeForNewHitself = 0;
			}
		}

	}

	target->timeForNewHitself += elapsedTime;
}

void Deerchant::behaviorWithStatic(WorldObject* target, float elapsedTime)
{	
	if (target->tag == Tag::wreathTable && Helper::getDist(position, target->getPosition()) <= radius + target->getRadius())
		nearTheTable = true;
	if (target->tag == Tag::fog && target->getState() == common)
		if (abs(position.x - target->getPosition().x) <= target->getConditionalSizeUnits().x / 2.5 && abs(position.y - target->getPosition().y) <= target->getConditionalSizeUnits().y / 2.5)
			target->setState(absorbed);
}

void Deerchant::behavior(float elapsedTime)
{		
	jerkInteract(elapsedTime);
	endingPreviousAction();
	fightLogic(elapsedTime);

	if (!boundTarget || boundTarget->isProcessed)
	{
		movePosition = Vector2f(-1, -1);
		return;
	}

	if (currentAction != jerking && boundTarget->isProcessed)
		movePosition = boundTarget->getPosition();

	bool isIntersect = (Helper::getDist(position, movePosition)) <= (this->radius + boundTarget->getRadius());

	//touch selected object 
	if (isIntersect)
	{
		boundTarget->isProcessed = true;
		switch (boundTarget->tag)
		{
		case Tag::tree:
		{
			if (boundTarget->getState() == absorbed)
				break;
			changeAction(absorbs, true, false);
			currentSprite[0] = 1;
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->setState(absorbed);
			boundTarget->isProcessed = false;
			stopping(true);
			break;
		}
		case Tag::fern:
		{
			if (!boundTarget->inventory.empty())
			{
				changeAction(open, true, false);
				unsealInventoryOwner = boundTarget;
				boundTarget->isProcessed = false;
				stopping(true, true);
			}
			else
			{
				changeAction(grab, true, false);
				stopping(true);
			}
			
			break;
		}
		case Tag::chamomile:
		case Tag::yarrow:		
		case Tag::mugwort:
		case Tag::noose:
		case Tag::droppedLoot:
		case Tag::hareTrap:
		case Tag::heroBag:
		{
			changeAction(grab, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			stopping(true);
			break;
		}
		case Tag::dropPoint:
		{		
			changeAction(dropping, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		case Tag::buildObject:
		{
			changeAction(builds, true, false);
			side = calculateSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			changeAction(relax, true, false);
			if (boundTarget)
			{
				boundTarget->isProcessed = false;
				side = calculateSide(boundTarget->getPosition(), elapsedTime);
				stopping(true, true);
			}
			break;
		}
		}
	}
	else
	{
		boundTarget->isProcessed = false;
	}
	//----------------------------	
}

void Deerchant::onMouseUp(int currentMouseButton, WorldObject *mouseSelectedObject, Vector2f mouseWorldPos, bool isBuilding)
{
	if (isBuilding && currentMouseButton == 2)
	{
		if (boundTarget != nullptr)
		{
			boundTarget->isProcessed = false;
			boundTarget = nullptr;			
		}
		boundTarget = new EmptyObject("buildItem", mouseWorldPos);
		boundTarget->tag = Tag::buildObject;
		movePosition = mouseWorldPos;
		return;
	}

	if (mouseSelectedObject && mouseSelectedObject->tag == Tag::brazier)
	{
		auto brazier = dynamic_cast<Brazier*>(mouseSelectedObject);
		if (heldItem->content.first != Tag::emptyCell &&
			Helper::getDist(brazier->getPlatePosition(), position) <= brazier->getPlateRadius() + radius)
		{
			brazier->putItemToCraft(heldItem->content.first);
			heldItem->content = heldItem->content.second > 1
				? std::make_pair(heldItem->content.first, heldItem->content.second - 1)
				: std::make_pair(Tag::emptyCell, 0);
		}
		return;
	}

	if (!mouseSelectedObject)
	{
		if (heldItem->content.first != Tag::emptyCell && currentMouseButton == 2)
		{
			if (boundTarget != nullptr)
			{
				boundTarget->isProcessed = false;
				boundTarget = nullptr;				
			}
			boundTarget = new EmptyObject("droppedItem", mouseWorldPos);
			boundTarget->tag = Tag::dropPoint;
			movePosition = mouseWorldPos;
			return;
		}

		for (auto& bag : bags)
		{
			if (bag.currentState == ejected)
			{
				if (boundTarget != nullptr)
				{
					boundTarget->isProcessed = false;
					boundTarget = nullptr;
				}
				boundTarget = new EmptyObject("droppedBag", mouseWorldPos);
				boundTarget->tag = Tag::dropPoint;
				movePosition = mouseWorldPos;
			}
		}
		return;
	}	

	if (currentMouseButton == 2)
	{
		boundTarget = mouseSelectedObject;
		movePosition = mouseSelectedObject->getPosition();
	}
}

void Deerchant::endingPreviousAction()
{
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
		changeAction(relax, true, false);
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
		changeAction(relax, true, false);
	if (lastAction == open)
		changeAction(relax, true, false);
	if (lastAction == absorbs)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
		changeAction(relax, true, false);
	}
	if (lastAction == builds)
		changeAction(relax, true, false);
	if (lastAction == dropping)
	{
		stopping(true, true);
		changeAction(relax, true, false);
		if (heldItem->content.first != Tag::emptyCell)
		{
			birthStaticInfo dropObject;
			dropObject.position = { position.x, position.y + radius };
			dropObject.tag = Tag::droppedLoot;
			dropObject.typeOfObject = int(heldItem->content.first);
			dropObject.count = heldItem->content.second;
			birthStatics.push(dropObject);
			heldItem->content = std::make_pair(Tag::emptyCell, 0);
		}
		else
		{
			for (int cnt = 0; cnt != bags.size(); cnt++)			
				if (bags[cnt].currentState == ejected)
				{
					bool isHareTrap = true;
					for (auto& item : bags[cnt].cells)					
						if (item.content.first != Tag::yarrow && item.content.first != Tag::emptyCell)
						{
							isHareTrap = false;
							break;
						}
					birthStaticInfo dropObject;
					if (isHareTrap)
					{											
						dropObject.tag = Tag::hareTrap;											
					}
					else
					{
						dropObject.tag = Tag::droppedLoot;
						dropObject.typeOfObject = int(Tag::heroBag);	
					}
					dropObject.position = { position.x, position.y + radius };
					dropObject.inventory = HeroBag::cellsToInventory(bags[cnt].cells);
					bags[cnt].cleanTextureReferences();
					//bags[cnt].~HeroBag();
					bags.erase(bags.begin() + cnt);
					birthStatics.push(dropObject);
					break;
				}	
		}
	}
	if (currentAction == throwNoose && currentSprite[0] == 12 && heldItem->content == std::make_pair(Tag::noose, 1))
	{		
		heldItem->content = { Tag::emptyCell, 0 };
		birthDynamicInfo nooseObject;
		nooseObject.position = position;
		nooseObject.tag = Tag::noose;
		birthDynamics.push(nooseObject);
	}
    if (lastAction == throwNoose)
    {
		changeAction(relax, true, false);
    }
	if (lastAction == open)
	{
		if (unsealInventoryOwner)
		{
			unsealInventoryOwner->isVisibleInventory = true;
			changeAction(relax, true, false);
		}
	}
	if (lastAction == grab)
	{
		if (boundTarget)
		{
			auto pickedItem = dynamic_cast<PickedObject*>(boundTarget);
			if (pickedItem)
			{
				if (pickedItem->getType() == int(Tag::heroBag) || pickedItem->getId() == Tag::hareTrap)
				{
					const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
						closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
					bags.resize(bags.size() + 1);
					bags[bags.size() - 1] = *(new HeroBag());
					bags[bags.size()-1].initialize(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2), true, pickedItem->inventory);
					pickedItem->deletePromiseOn();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			auto nooseItem = dynamic_cast<Noose*>(boundTarget);
			if (nooseItem)
			{
				auto placedNoose = new std::vector<std::pair<Tag, int>>({ (std::make_pair(Tag::noose, 1)) });
				if (HeroBag::putItemsIn(placedNoose, &bags))
					nooseItem->deletePromiseOn();
				delete placedNoose;
			}
			stopping(true, true);
			changeAction(relax, true, false);
		}
	}
	lastAction = relax;
}

void Deerchant::jerkInteract(float elapsedTime)
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
		{
			changeAction(relax, true, false);
			speed = defaultSpeed;
		}
	}
}

void Deerchant::stopping(bool doStand, bool forgetSelectedTarget, bool offUnsealInventory)
{
	if (boundTarget != nullptr && currentAction != dropping)
		if (boundTarget->getName() == "droppedBag")
			for (auto& bag : bags)
				if (bag.currentState == ejected)
					bag.currentState = bagClosed;

	if (doStand)
	{
		this->movePosition = { -1, -1 };
		moveOffset = { 0, 0 };
		this->direction = STAND;
	}

	if (forgetSelectedTarget && boundTarget != nullptr)
	{
		boundTarget->isProcessed = false;
		boundTarget = nullptr;
	}

	if (offUnsealInventory)
	{
		if (unsealInventoryOwner != nullptr)
			unsealInventoryOwner->isVisibleInventory = false;
		unsealInventoryOwner = nullptr;
	}
}

Vector2f Deerchant::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

Vector2f Deerchant::getBeltPosition()
{
	if (additionalSprites.size() >= 2) return
		Vector2f((4 * additionalSprites[0].position.x + additionalSprites[1].position.x) / 5.0f + conditionalSizeUnits.x / 3.0f,
		(4 * additionalSprites[0].position.y + additionalSprites[1].position.y) / 5.0f);
	return additionalSprites[0].position;
}

int Deerchant::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void Deerchant::jerk(float power, float deceleration, Vector2f destinationPoint)
{
	stopping(false, true);
	this->jerkPower = power;
	this->jerkDeceleration = deceleration;
	this->jerkDuration = 700000;
	this->jerkTime = this->jerkDuration;
	changeAction(jerking, currentAction != jerking, false);
	jerkDistance = 500;
	currentSprite[0] = 1;

	movePosition = Vector2f(position.x + cos(direction * pi / 180) * jerkDistance, position.y - sin(direction * pi / 180) * jerkDistance);
}

void Deerchant::fightLogic(float elapsedTime, DynamicObject* target)
{
	pushAway(elapsedTime);
}

void Deerchant::prepareSpriteNames(long long elapsedTime, float scaleFactor)
{
	spriteChainElement legsSprite, bodySprite, fullSprite;
	bool isInverse = false;
	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
	legsSprite.animationLength = 8;
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2f(this->conditionalSizeUnits);
	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	std::string sideStr = DynamicObject::sideToString(side), directionStr = DynamicObject::directionToString(direction);
	animationSpeed = 12;
	if (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT)
	{
		directionStr = "left";
		legsSprite.mirrored = true;
	}
	if (side == right && currentAction != move && currentAction != jerking)
	{
		sideStr = "left";
		bodySprite.mirrored = true;
	}
	additionalSprites.clear();

	switch (currentAction)
	{
	case commonHit:
		animationLength = 8;
		if (side == right)		
			legsSprite.mirrored = true;	
		bodySprite.path = "Game/worldSprites/hero/hit/body/" + sideStr + '/';
		legsSprite.path = "Game/worldSprites/hero/hit/legs/" + sideStr + '/';
		break;
	case absorbs:
		animationLength = 10;
		bodySprite.path = "Game/worldSprites/hero/absorb/" + sideStr + '/';		
		break;
	case builds:
		animationLength = 10;
		bodySprite.path = "Game/worldSprites/hero/absorb/" + sideStr + '/';		
		break;
	case grab:
		animationLength = 11;
		animationSpeed = 15;
		bodySprite.path = "Game/worldSprites/hero/pick/" + sideStr + '/';
		break;
	case dropping:
		isInverse = true;
		animationLength = 8;
		bodySprite.path = "Game/worldSprites/hero/pick/" + sideStr + '/';
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		break;
	case jerking:
		animationLength = 8;
		animationSpeed = 11;
		if (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT)
			bodySprite.mirrored = true;
		bodySprite.path = "Game/worldSprites/hero/roll/" + directionStr + '/';		
		break;
	case open:
		animationLength = 12;
		bodySprite.path = "Game/worldSprites/hero/pick/" + sideStr + '/';
		break;
	case relax:
		animationLength = 16;
		animationSpeed = 13;
		bodySprite.path = "Game/worldSprites/hero/stand/" + sideStr + '/';
		break;
	}

	if (currentAction == move)
	{
		animationLength = 8;
		if (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT)
			bodySprite.mirrored = true;
		bodySprite.path = "Game/worldSprites/hero/move/body/" + directionStr + '/';
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + directionStr + '/';		
	}

	if (currentAction == moveHit)
	{
		animationLength = 8;		

		if (direction == UP && side == down || direction == DOWN && side == up )
		{
			isInverse = true;
			directionStr = sideStr;			
		}
		if ((direction == LEFT || direction == UPLEFT || direction == DOWNLEFT) && side == right ||
			(direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == left)
		{
			isInverse = true;
			legsSprite.mirrored = !legsSprite.mirrored;
		}

		legsSprite.path = "Game/worldSprites/hero/move/legs/" + directionStr + '/';
		bodySprite.path = "Game/worldSprites/hero/hit/body/" + sideStr + '/';

		if (direction == STAND)
		{
			if (side == right)
				legsSprite.mirrored = true;
			legsSprite.path = "Game/worldSprites/hero/hit/legs/" + sideStr + "/";
			legsSprite.animationLength = 14;
		}
	}

	if (currentAction == throwNoose)
	{
		animationLength = 14;
		if (direction == UP && side == down || direction == DOWN && side == up)
		{
			isInverse = true;
			directionStr = sideStr;
		}
		if ((direction == LEFT || direction == UPLEFT || direction == DOWNLEFT) && side == right ||
			(direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == left)
		{
			isInverse = true;
			legsSprite.mirrored = !legsSprite.mirrored;
		}
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + directionStr + '/';
		bodySprite.path = "Game/worldSprites/hero/throw/body/" + sideStr + '/';

		if (direction == STAND)
		{
			if (side == right)
				legsSprite.mirrored = true;
			legsSprite.path = "Game/worldSprites/hero/throw/legs/" + sideStr + "/";
			legsSprite.animationLength = 14;
		}
	}

	if (!legsSprite.path.empty())
	{
		if (isInverse)
			legsSprite.path += std::to_string(legsSprite.animationLength + 1 - currentSprite[1]) + ".png";
		else
			legsSprite.path += std::to_string(currentSprite[1]) + ".png";
		additionalSprites.push_back(legsSprite);
	}
	if (!bodySprite.path.empty())
	{
		bodySprite.path += std::to_string(currentSprite[0]) + ".png";
		bodySprite.offset.y += conditionalSizeUnits.y / 10.0f;
		bodySprite.position = Vector2f(position.x, position.y + conditionalSizeUnits.y / 10.0f);
		additionalSprites.push_back(bodySprite);
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 1000000 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite[0] > animationLength)
		{
			lastAction = currentAction;
			currentSprite[0] = 1;
		}
		if (++currentSprite[1] > legsSprite.animationLength)		
			currentSprite[1] = 1;		
	}
}
