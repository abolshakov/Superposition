#include "Deerchant.h"
#include "Helper.h"
#include "PickedObject.h"
#include "Noose.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite[0] = 1;
	currentSprite.resize(2);
	timeForNewSprite = 0;
	defaultSpeed = 0.0007f;
	speed = defaultSpeed;
	animationSpeed = 0.0010f;
	animationLength = 8;	
	radius = 50;
	strength = 20;
	maxHealthPointValue = 1000;
	healthPoint = maxHealthPointValue;
	energy = 50; maxEnergyValue = 100;
	currentAction = relax;
	toSaveName = "this1";
	tag = Tag::hero1;
	canCrashIntoDynamic = false;

	bags.resize(3);
	const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
		closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
	bags[0].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y), closedBagSize,openBagSize, true);
	bags[1].initialize(Vector2f(Helper::GetScreenSize().x - closedBagSize.x, Helper::GetScreenSize().y - closedBagSize.y), closedBagSize, openBagSize, true);
	bags[2].initialize(Vector2f(Helper::GetScreenSize().x / 3 - closedBagSize.x, Helper::GetScreenSize().y / 2 - closedBagSize.y), closedBagSize, openBagSize, true);
}

Deerchant::~Deerchant()
{

}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2i(300, 375);
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i(textureBox.width / 2, textureBox.height * 4 / 5);
}

void Deerchant::handleInput()
{
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == dropping || currentAction == builds || currentAction == jerking)
		return;

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && direction != STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveOffset = Vector2f(-1, -1);
		
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
					currentAction = relax;
			}
			else
			{
				if (boundTarget)
				{
					setMoveOffset(0);
					currentAction = move;
				}
			}
		}
	}
	else
		return;

	//define actions
	if (Keyboard::isKeyPressed(Keyboard::E))
		changeAction(openInventory, true, false);

	if (Mouse::isButtonPressed(Mouse::Left) && heldItem->content.first == Tag::noose)
	{
		changeAction(throwNoose, true, false);
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D))
		currentAction = move;

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (boundTarget != nullptr)
			stopping(true, true);
	}
	//--------------

	if (isBuildSystemMaker)
		return;
	if (Keyboard::isKeyPressed(Keyboard::Z) && (currentAction == relax || currentAction == combatState))
	{
		currentAction = transitionToEnotherWorld;
		currentSprite[0] = 1;
	}
	else
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (currentAction == move)
				currentAction = moveHit;
			else
			{
				currentAction = commonHit;
			}
		}
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
	return;
}

void Deerchant::behaviorWithDynamic(DynamicObject* target, float elapsedTime)
{	
	bool isIntersect = (sqrt(pow(this->position.x - target->getPosition().x, 2) + pow(this->position.y - target->getPosition().y, 2)) <= (this->radius + target->getRadius()) + 10);

	if (isIntersect && target->isSelected/* gettargetSide(thisObject, target) == this->hitDirection*/)
	{
		if (target->timeForNewHitself >= target->getTimeAfterHitself())
		{
			if (this->currentAction == commonHit && this->getSpriteNumber() == 4)
			{
				this->addEnergy(5);
				target->takeDamage(this->getStrength());
				target->timeForNewHitself = 0;
			}
		}

	}

	target->timeForNewHitself += elapsedTime;
}

void Deerchant::behaviorWithStatic(WorldObject* target, float elapsedTime)
{
	if (target->tag == Tag::fog && target->getState() == common)
	{
		if (abs(position.x - target->getPosition().x) <= target->getConditionalSizeUnits().x / 2.5 && abs(position.y - target->getPosition().y) <= target->getConditionalSizeUnits().y / 2.5)
			target->setState(absorbed);
	}
}

void Deerchant::behavior(float elapsedTime)
{		
	jerkInteract(elapsedTime);
	endingPreviousAction();

	if (!boundTarget || boundTarget->isProcessed)
	{
		movePosition = Vector2f(-1, -1);
		return;
	}

	if (currentAction != jerking)
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
			currentAction = absorbs;
			currentSprite[0] = 1;
			setSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->setState(absorbed);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		case Tag::chamomile:
		case Tag::yarrow:
		case Tag::noose:
		case Tag::droppedLoot:
		case Tag::hareTrap:
		case Tag::heroBag:
		{
			currentAction = grab;
			currentSprite[0] = 1;
			stopping(true);
			break;
		}
		case Tag::dropPoint:
		{		
			currentAction = dropping;
			currentSprite[0] = 1;
			setSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		case Tag::buildObject:
		{
			currentAction = builds;
			currentSprite[0] = 1;
			setSide(boundTarget->getPosition(), elapsedTime);
			boundTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			currentAction = relax;
			if (boundTarget)
			{
				boundTarget->isProcessed = false;
				setSide(boundTarget->getPosition(), elapsedTime);
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

void Deerchant::onMouseDownBehavior(int currentMouseButton, WorldObject *object, Vector2f mouseWorldPos, bool isBuilding)
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

	if (!object)
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
				boundTarget = new EmptyObject("droppedItem", mouseWorldPos);
				boundTarget->tag = Tag::dropPoint;
				movePosition = mouseWorldPos;
			}
		}
		return;
	}	

	if (currentMouseButton == 2)
	{
		boundTarget = object;
		movePosition = object->getPosition();
	}
}

void Deerchant::endingPreviousAction()
{
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
		currentAction = relax;
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
		currentAction = relax;
	if (lastAction == openInventory)
		currentAction = relax;  
	if (lastAction == absorbs)
		currentAction = relax;
	if (lastAction == builds)
		currentAction = relax;
	if (lastAction == dropping)
	{
		stopping(true, true);
		currentAction = relax;
		if (heldItem->content.first != Tag::emptyCell)
		{
			birthStaticInfo dropObject;
			dropObject.position = position;
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
					dropObject.position = position;
					dropObject.inventory = HeroBag::cellsToInventory(bags[cnt].cells);
					bags.erase(bags.begin() + cnt);
					birthStatics.push(dropObject);
					break;
				}	
		}
	}
    if (lastAction == throwNoose)
    {
		if (HeroBag::canAfford({ std::make_pair(Tag::noose, 1) }, &bags), heldItem)
		{
			HeroBag::takeItems({ std::make_pair(Tag::noose, 1) }, &bags, heldItem);
			birthDynamicInfo nooseObject;
			nooseObject.position = position;
			nooseObject.tag = Tag::noose;
			birthDynamics.push(nooseObject);
		}
		currentAction = relax;
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
					bags[bags.size()-1].initialize(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2), closedBagSize, openBagSize, true, pickedItem->inventory);
					pickedItem->deletePromiseOn();
				}
				else
					pickedItem->pickUp(&this->bags);
			}
			
			auto nooseItem = dynamic_cast<Noose*>(boundTarget);
			if (nooseItem)
			{
				if (HeroBag::putItemsIn({ std::make_pair(Tag::noose, 1) }, &bags))
					nooseItem->deletePromiseOn();
			}
			stopping(true, true);
			currentAction = relax;
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
			currentAction = relax;
			speed = defaultSpeed;
		}
	}
}

void Deerchant::stopping(bool doStand, bool forgetSelectedTarget)
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
	currentAction = jerking;
	jerkDistance = 500;
	currentSprite[0] = 1;

	movePosition = Vector2f(position.x + cos(direction * pi / 180) * jerkDistance, position.y - sin(direction * pi / 180) * jerkDistance);
}

void Deerchant::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement legsSprite, bodySprite, fullSprite;

	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
	legsSprite.animationLength = 8;
	bodySprite.offset = Vector2f(this->textureBoxOffset);
	bodySprite.size = Vector2f(this->conditionalSizeUnits);
	fullSprite.offset = Vector2f(this->textureBoxOffset);
	fullSprite.size = Vector2f(this->conditionalSizeUnits);
	
	additionalSprites.clear();

	switch (currentAction)
	{
	case commonHit:
		animationLength = 8;
		animationSpeed = 0.0005f;
		bodySprite.path = "Game/worldSprites/hero/hit/body/" + DynamicObject::sideToString(side) + '/';
		legsSprite.path = "Game/worldSprites/hero/hit/legs/" + DynamicObject::sideToString(side) + '/';		
		break;
	case absorbs:
		animationLength = 19;
		animationSpeed = 0.0010f;
		bodySprite.path = "Game/worldSprites/hero/absorb/" + DynamicObject::sideToString(side) + '/';		
		break;
	case builds:
		animationLength = 19;
		animationSpeed = 0.0010f;
		bodySprite.path = "Game/worldSprites/hero/absorb/" + DynamicObject::sideToString(side) + '/';
		break;
	case grab:
		animationLength = 12;
		animationSpeed = 0.0009f;
		bodySprite.path = "Game/worldSprites/hero/grab/" + DynamicObject::sideToString(side) + '/';
		break;
	case dropping:
		animationLength = 6;
		animationSpeed = 0.0006f;
		bodySprite.path = "Game/worldSprites/hero/grab/" + DynamicObject::sideToString(side) + '/';
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		animationSpeed = 0.0010f;
		break;
	case jerking:
		animationLength = 8;
		animationSpeed = (1 / (jerkDuration / animationLength)) * 40;
		bodySprite.path = "Game/worldSprites/hero/roll/" + DynamicObject::directionToString(direction) + '/';		
		break;
	case openInventory:
		animationLength = 12;
		animationSpeed = 0.0010f;
		break;
	case relax:
		animationLength = 1;
		currentSprite[0] = 1;
		animationSpeed = 0.0010f;
		bodySprite.path = "Game/worldSprites/hero/stand/" + DynamicObject::sideToString(side) + '/';
		break;
	}

	if (currentAction == move)
	{
		animationLength = 8;
		animationSpeed = 0.0005f;
		bodySprite.path = "Game/worldSprites/hero/move/body/" + DynamicObject::directionToString(direction) + '/';
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + DynamicObject::directionToString(direction) + '/';
	}

	if (currentAction == moveHit)
	{
		animationLength = 8;
		animationSpeed = 0.0005f;
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + DynamicObject::directionToString(direction) + '/';
		bodySprite.path = "Game/worldSprites/hero/hit/body/" + DynamicObject::sideToString(side) + '/';
		if (direction == UP && side == down || (direction == LEFT || direction == UPLEFT || direction == DOWNLEFT) && side == right ||
			direction == DOWN && side == up || (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == left)
			legsSprite.path = "Game/worldSprites/hero/move/legs/" + DynamicObject::sideToString(side) + "Inverse/";
		if ((direction == LEFT || direction == UPLEFT ||direction == DOWNLEFT) && side == up)
			bodySprite.path = "Game/worldSprites/hero/hit/body/upRight/";
		if ((direction == LEFT || direction == UPLEFT || direction == DOWNLEFT) && side == down)
			bodySprite.path = "Game/worldSprites/hero/hit/body/down/";
		if ((direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == up)
			bodySprite.path = "Game/worldSprites/hero/hit/body/up/";
		if ((direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == down)
			bodySprite.path = "Game/worldSprites/hero/hit/body/downRight/";
	}

	if (currentAction == throwNoose)
	{
		animationLength = 12;
		animationSpeed = 0.0005f;
		legsSprite.path = "Game/worldSprites/hero/move/legs/" + DynamicObject::directionToString(direction) + '/';		
		bodySprite.path = "Game/worldSprites/hero/throw/body/" + DynamicObject::sideToString(side) + '/';
		if (direction == UP && side == down || (direction == LEFT || direction == UPLEFT || direction == DOWNLEFT) && side == right ||
			direction == DOWN && side == up || (direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT) && side == left)
			legsSprite.path = "Game/worldSprites/hero/move/legs/" + DynamicObject::sideToString(side) + "Inverse/";
		if (direction == STAND)
			legsSprite.path = "Game/worldSprites/hero/throw/legs/" + DynamicObject::sideToString(side) + '/';
	}

	if (!legsSprite.path.empty())
	{
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

	if (timeForNewSprite >= 40 / animationSpeed)
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
