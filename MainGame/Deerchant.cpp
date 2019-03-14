#include "Deerchant.h"
#include "Helper.h"
#include "PickedObject.h"
#include "Noose.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
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
	currentWorld = "common";
	toSaveName = "this1";
	tag = mainHeroTag;
	canCrashIntoDynamic = false;

	bags.resize(1);
	const Vector2f openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3),
		closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
	bags[0].initialize(
		"Game/inventorySprites/bag1Icon.png",
		"Game/inventorySprites/bag1IconHover.png", 
		"Game/inventorySprites/bag1.png",
		"Game/inventorySprites/bag1Hover.png",
		Vector2f(Helper::GetScreenSize().x - closedBagSize.x, closedBagSize.y),
		closedBagSize,
		openBagSize,
		true);
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

void Deerchant::handleInput()
{
	//high-priority actions
	if (currentAction == absorbs || currentAction == grab || currentAction == builds || currentAction == jerking || currentAction == throwNoose)
		return;

	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != jerking && direction != STAND)
	{
		jerk(3, 2);
		return;
	}
	//---------------------

	setHitDirection();
	moveOffset = Vector2f(-1, -1);

	if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
	{
		direction = UPLEFT;
	}
	else
		if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
		{
			direction = UPRIGHT;
		}
		else
			if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
			{
				direction = DOWNLEFT;
			}
			else
				if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
				{
					direction = DOWNRIGHT;
				}
				else
					if (Keyboard::isKeyPressed(Keyboard::A))
					{
						direction = LEFT;
					}
					else
						if (Keyboard::isKeyPressed(Keyboard::D))
						{
							direction = RIGHT;
						}
						else
							if (Keyboard::isKeyPressed(Keyboard::W))
							{
								direction = UP;
							}
							else
								if (Keyboard::isKeyPressed(Keyboard::S))
								{
									direction = DOWN;
								}
								else
								{
									bool isIntersect = false;
									if (selectedTarget)
										isIntersect = (sqrt(pow(this->position.x - movePosition.x, 2) + pow(this->position.y - movePosition.y, 2)) <= (this->radius + selectedTarget->getRadius()));
									if (isIntersect || !selectedTarget)
									{
										direction = STAND;
										if (currentAction == move)
											currentAction = relax;
									}
									else
									{
										if (selectedTarget)
										{
											setMoveOffset(0);
											currentAction = move;
										}
									}
								}

	//define actions
	if (Keyboard::isKeyPressed(Keyboard::E))
		currentAction = openInventory;

    if (Keyboard::isKeyPressed(Keyboard::R))
		currentAction = throwNoose;

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D))
		currentAction = move;

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::R) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (selectedTarget != nullptr)
			stopping(true, true);
	}
	//--------------

	if (isBuildSystemMaker)
		return;
	if (Keyboard::isKeyPressed(Keyboard::Z) && (currentAction == relax || currentAction == combatState))
	{
		currentAction = transitionToEnotherWorld;
		currentSprite = 1;
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

void Deerchant::behaviorWithDynamic(DynamicObject& target, float elapsedTime)
{
	bool isIntersect = (sqrt(pow(this->position.x - target.getPosition().x, 2) + pow(this->position.y - target.getPosition().y, 2)) <= (this->radius + target.getRadius()) + 10);

	if (target.getCurrentAction() == dead)
	{
		if (isIntersect && this->lastAction == openInventory && target.inventory.size() != 0)
			target.setInventoryVisibility(true);
		if (this->direction != STAND)
			target.setInventoryVisibility(false);
		return;
	}

	if (isIntersect && target.isSelected/* gettargetSide(thisObject, target) == this->hitDirection*/)
	{
		if (target.timeForNewHitself >= target.getTimeAfterHitself())
		{
			if (this->currentAction == commonHit && this->getSpriteNumber() == 4)
			{
				this->addEnergy(5);
				target.takeDamage(this->getStrength());
				target.timeForNewHitself = 0;
			}
		}

	}

	target.timeForNewHitself += elapsedTime;
}

void Deerchant::behaviorWithStatic(WorldObject& target, float elapsedTime)
{

}

void Deerchant::behavior(float elapsedTime)
{	
	jerkInteract(elapsedTime);
	endingPreviousAction();

	if (!selectedTarget || selectedTarget->isProcessed)
	{
		movePosition = Vector2f(-1, -1);
		return;
	}

	if (currentAction != jerking)
		movePosition = selectedTarget->getPosition();

	bool isIntersect = (Helper::getDist(position, movePosition)) <= (this->radius + selectedTarget->getRadius());

	//touch selected object 
	if (isIntersect)
	{
		selectedTarget->isProcessed = true;
		switch (selectedTarget->tag)
		{
		case forestTreeTag:
		{
			if (selectedTarget->getState() == absorbed)
				break;

			currentAction = absorbs;
			currentSprite = 1;
			setSide(selectedTarget->getPosition(), elapsedTime);
			selectedTarget->setState(absorbed);
			selectedTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		case chamomileTag:
		{
			currentAction = grab;
			currentSprite = 1;
			stopping(true);
			break;
		}
		case yarrowTag:
		{
			currentAction = grab;
			currentSprite = 1;
			stopping(true);
			break;
		}
		case buildedObjectTag:
		{
			currentAction = builds;
			currentSprite = 1;
			setSide(selectedTarget->getPosition(), elapsedTime);
			selectedTarget->isProcessed = false;
			stopping(true, true);
			break;
		}
		default:
		{
			currentAction = relax;
			if (selectedTarget)
			{
				selectedTarget->isProcessed = false;
				setSide(selectedTarget->getPosition(), elapsedTime);
				stopping(true, true);
			}
			break;
		}
		}
	}
	else
	{
		selectedTarget->isProcessed = false;
	}
	//----------------------------
}

void Deerchant::onMouseDownBehavior(WorldObject *object, Vector2f mouseWorldPos, bool isBuilding)
{
	if (isBuilding)
	{
		if (selectedTarget != nullptr)
		{
			selectedTarget = nullptr;
			selectedTarget->isProcessed = false;
		}
		selectedTarget = new EmptyObject("buildItem", mouseWorldPos);
		selectedTarget->tag = buildedObjectTag;
		movePosition = mouseWorldPos;
		return;
	}

	if (!object)
		return;
	if (selectedTarget != nullptr)
		if (selectedTarget->isProcessed)
			return;

	selectedTarget = object;
	movePosition = object->getPosition();
}

void Deerchant::endingPreviousAction()
{
	if (lastAction == transitionToEnotherWorld)
	{
		if (currentWorld == "common")
			currentWorld = "spirit";
		else
			currentWorld = "common";
		currentAction = relax;
	}
	if (lastAction == commonHit && !Mouse::isButtonPressed(Mouse::Left))
		currentAction = relax;
	if (lastAction == moveHit && !Mouse::isButtonPressed(Mouse::Left))
		currentAction = relax;
	if (lastAction == openInventory)
		currentAction = relax;
    if (lastAction == throwNoose)
		currentAction = relax;
	if (lastAction == absorbs)
		currentAction = relax;
	if (lastAction == builds)
		currentAction = relax;
    if (currentAction == throwNoose)
    {
        DynamicObject* noose = nullptr; 
        noose = new Noose("noose", position);
    }
	if (lastAction == grab)
	{
		if (selectedTarget)
		{
			auto item = dynamic_cast<PickedObject*>(selectedTarget);
			if (item)
				item->pickUp(&this->bags);
			stopping(true, true);
			currentAction = relax;
		}
	}
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

	if (forgetSelectedTarget && selectedTarget != nullptr)
	{
		selectedTarget->isProcessed = false;
		selectedTarget = nullptr;
	}
}

Vector2f Deerchant::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
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
	currentSprite = 1;

	movePosition = Vector2f(position.x + cos(direction * pi / 180) * jerkDistance, position.y - sin(direction * pi / 180) * jerkDistance);
}

void Deerchant::prepareSpriteNames(long long elapsedTime)
{
	spriteChainElement legsSprite, bodySprite, fullSprite;

	legsSprite.offset = Vector2f(this->textureBoxOffset);
	legsSprite.size = Vector2f(this->conditionalSizeUnits);
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

		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/upLeft/";
				legsSprite.path = "Game/worldSprites/hero/hit/legs/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/right/";
				legsSprite.path = "Game/worldSprites/hero/hit/legs/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/downLeft/";
				legsSprite.path = "Game/worldSprites/hero/hit/legs/down/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/left/";
				legsSprite.path = "Game/worldSprites/hero/hit/legs/left/";
				break;
			}
		}
		break;
	case absorbs:
		animationLength = 19;
		animationSpeed = 0.0010f;
		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/down/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/left/";
				break;
			}
		}
		break;
	case builds:
		animationLength = 19;
		animationSpeed = 0.0010f;
		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/down/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/absorb/left/";
				break;
			}
		}
		break;
	case grab:
		animationLength = 12;
		animationSpeed = 0.0009f;

		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/down/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/left/";
				break;
			}
		}
		break;
    case throwNoose:
		animationLength = 12;
		animationSpeed = 0.0009f;

		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/down/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/grab/left/";
				break;
			}
		}
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		animationSpeed = 0.0010f;
		break;
	case jerking:
		animationLength = 8;
		animationSpeed = (1 / (jerkDuration / animationLength)) * 40;

		switch (direction)
		{
			case UP:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/up/";
				break;
			}
			case RIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/right/";
				break;
			}
			case LEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/left/";
				break;
			}
			case DOWN:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/down/";
				break;
			}
			case UPLEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/left/";
				break;
			}
			case UPRIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/right/";
				break;
			}
			case DOWNLEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/left/";
				break;
			}
			case DOWNRIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/roll/right/";
				break;
			}
			case STAND:
			{
				animationLength = 1;
				currentSprite = 1;
				animationSpeed = 0.0010f;
				switch (side)
				{
					case up:
					{
						bodySprite.path = "Game/worldSprites/hero/stand/up/";
						break;
					}
					case right:
					{
						bodySprite.path = "Game/worldSprites/hero/stand/right/";
						break;
					}
					case down:
					{
						bodySprite.path = "Game/worldSprites/hero/stand/down/";
						break;
					}
					case left:
					{
						bodySprite.path = "Game/worldSprites/hero/stand/left/";
						break;
					}
				}
			}
		}
		break;
	case openInventory:
		animationLength = 12;
		animationSpeed = 0.0010f;
		break;
	case relax:
		animationLength = 1;
		currentSprite = 1;
		animationSpeed = 0.0010f;

		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/stand/up/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/stand/right/";				
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/stand/down/";				
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/stand/left/";				
				break;
			}
		}
		break;
	}

	if (currentAction == move)
	{
		animationLength = 8;
		animationSpeed = 0.0005f;

		switch (direction)
		{
			case UP:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/up/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/up/";
				break;
			}
			case RIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/right/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
			case LEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/left/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case DOWN:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/down/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/down/";
				break;
			}
			case UPLEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/left/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case UPRIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/right/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
			case DOWNLEFT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/left/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case DOWNRIGHT:
			{
				bodySprite.path = "Game/worldSprites/hero/move/body/right/";
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
		}
	};

	if (currentAction == moveHit)
	{
		animationLength = 8;
		animationSpeed = 0.0005f;

		switch (direction)
		{
			case UP:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/up/";
				break;
			}
			case RIGHT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
			case LEFT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case DOWN:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/down/";
				break;
			}
			case UPLEFT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case UPRIGHT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
			case DOWNLEFT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/left/";
				break;
			}
			case DOWNRIGHT:
			{
				legsSprite.path = "Game/worldSprites/hero/move/legs/right/";
				break;
			}
		}
		switch (side)
		{
			case up:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/upLeft/";
				break;
			}
			case right:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/right/";
				break;
			}
			case down:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/downLeft/";
				break;
			}
			case left:
			{
				bodySprite.path = "Game/worldSprites/hero/hit/body/left/";
				break;
			}
		}
	}

	legsSprite.path += std::to_string(currentSprite) + ".png";
	bodySprite.path += std::to_string(currentSprite) + ".png";
	if (legsSprite.path != "")
		additionalSprites.push_back(legsSprite);
	if (bodySprite.path != "")
		additionalSprites.push_back(bodySprite);

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			lastAction = currentAction;
			currentSprite = 1;
		}
	}
}
