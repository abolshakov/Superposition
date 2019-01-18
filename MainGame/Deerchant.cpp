#include "Deerchant.h"
#include "Helper.h"
#include "PickedObject.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationSpeed = 0.0010f;
	animationLength = 8;
	radius = 50;
	strength = 20;
	maxHealthPointValue = 1000;
	healthPoint = maxHealthPointValue;
	energy = 50; maxEnergyValue = 100; energyForSpecial = 20;
	currentAction = relax;
	currentWorld = "common";
	inventory.resize(16);
	for (auto curInvItem = inventory.begin(); curInvItem != inventory.end(); ++curInvItem)
	{
		curInvItem->first = 0;
		curInvItem->second = 0;
	}
	inventoryCapacity = 16;
	toSaveName = "this1";
	tag = mainHeroTag;

	inventory[0] = std::make_pair(1, 1);
	inventory[1] = std::make_pair(2, 2);
	inventory[2] = std::make_pair(1, 1);
	inventory[3] = std::make_pair(10, 1);
}

Deerchant::~Deerchant()
{

}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2i (250, 250);
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, textureBox.height * 13 / 16);
}

void Deerchant::handleInput()
{
	if (currentAction == absorbs || currentAction == grab || currentAction == builds)
		return;
	
	setHitDirection();
	moveOffset = Vector2f (-1, -1);

	if (Keyboard::isKeyPressed(Keyboard::E))
	{
		currentAction = openInventory;
	}

	if (currentAction != evasionDown && currentAction != evasionUp)
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D))
			currentAction = move;
	
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) ||
		Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LControl) ||
		Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		if (selectedTarget != nullptr)
		{
			selectedTarget->isProcessed = false;
			selectedTarget = nullptr;			
		}
		movePosition = Vector2f(-1, -1);
	}
	
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
	if (isBuildSystemMaker)
		return;
	if (Keyboard::isKeyPressed(Keyboard::Z) && (currentAction == relax || currentAction == combatState))
	{
		currentAction = transitionToEnotherWorld;
		currentSprite = 1;
	}
	else
	if (Mouse::isButtonPressed(Mouse::Left) && (currentAction == relax || currentAction == combatState))
	{
		currentAction = commonHit;
		currentSprite = 1;
	}
	else
	if (Keyboard::isKeyPressed(Keyboard::F) && (currentAction == relax || currentAction == combatState) && energy >= energyForSpecial)
	{
		energy -= energyForSpecial;
		currentAction = specialHit;
		currentSprite = 1;
	}
	else
	if (Keyboard::isKeyPressed(Keyboard::LControl) && currentAction != evasionDown && currentAction != evasionUp)
	{
		currentAction = evasionDown;
		direction = STAND;
		currentSprite = 1;
	}
	else
	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != evasionDown && currentAction != evasionUp)
	{
		currentAction = evasionUp;
		direction = STAND;
		currentSprite = 1;
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
			else
				if (this->currentAction == hardHit && this->getSpriteNumber() == 4)
				{
					this->addEnergy(15);
					target.takeDamage(this->getStrength()*1.5);
					target.timeForNewHitself = 0;
				}
				else
					if (this->currentAction == specialHit && this->getSpriteNumber() == 4)
					{
						target.takeDamage(this->getStrength() * 2);
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
	if (lastAction == transitionToEnotherWorld)
	{
		if (currentWorld == "common")
			currentWorld = "spirit";
		else
			currentWorld = "common";
		currentAction = relax;
	}
	if (lastAction == commonHit || lastAction == hardHit || lastAction == specialHit || lastAction == evasionDown || lastAction == evasionUp)
	{
		currentAction = relax;
	}
	if (lastAction == openInventory)
	{
		currentAction = relax;
	}
	if (lastAction == absorbs)
	{
		currentAction = relax;
	}
	if (lastAction == builds)
	{
		currentAction = relax;
	}
	if (lastAction == grab)
	{
		if (selectedTarget)
		{
			selectedTarget->isProcessed = false;
			auto item = dynamic_cast<PickedObject*>(selectedTarget);
			if (item)
				item->pickUp(this->inventory);
			selectedTarget = nullptr;
			movePosition = Vector2f(-1, -1);
			currentAction = relax;
		}
	}

	if (!selectedTarget || selectedTarget->isProcessed)
	{
		movePosition = Vector2f(-1, -1);
		return;
	}

	movePosition = selectedTarget->getPosition();

	bool isIntersect = (sqrt(pow(this->position.x - movePosition.x, 2) + pow(this->position.y - movePosition.y, 2)) <= (this->radius + selectedTarget->getRadius()));

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
				setSide(Vector2i(selectedTarget->getPosition()), elapsedTime);
				selectedTarget->setState(absorbed);
				selectedTarget->isProcessed = false;
				selectedTarget = nullptr;
				movePosition = Vector2f(-1, -1);
				direction = STAND;
				break;
			}
			case chamomileTag:
			{
				currentAction = grab;
				currentSprite = 1;
				direction = STAND;
				movePosition = Vector2f(-1, -1);
				break;
			}
			case yarrowTag:
			{
				currentAction = grab;
				currentSprite = 1;
				direction = STAND;
				movePosition = Vector2f(-1, -1);
				break;
			}
			case buildedObjectTag:
			{
				currentAction = builds;
				currentSprite = 1;
				setSide(Vector2i(selectedTarget->getPosition()), elapsedTime);
				selectedTarget->isProcessed = false;
				selectedTarget = nullptr;
				direction = STAND;
				movePosition = Vector2f(-1, -1);
				break;
			}
			default:
			{
				currentAction = relax;
				if (selectedTarget)
				{
					selectedTarget->isProcessed = false;
					setSide(Vector2i(selectedTarget->getPosition()), elapsedTime);
					selectedTarget = nullptr;
					movePosition = Vector2f(-1, -1);
				}					
				break;
			}
		}
	}
	else
	{
		selectedTarget->isProcessed = false;
	}
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

Vector2f Deerchant::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int Deerchant::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

std::string Deerchant::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
	case commonHit:
		animationLength = 18;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";		
		break;
	case hardHit:
		animationLength = 18;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case specialHit:
		animationLength = 18;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case absorbs:
		animationLength = 19;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/absorb/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/absorb/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/absorb/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/absorb/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case builds:
		animationLength = 19;
		switch (side)
		{
		case up:
		{
			spriteName = "Game/worldSprites/hero/absorb/up/";
			break;
		}
		case right:
		{
			spriteName = "Game/worldSprites/hero/absorb/right/";
			break;
		}
		case down:
		{
			spriteName = "Game/worldSprites/hero/absorb/down/";
			break;
		}
		case left:
		{
			spriteName = "Game/worldSprites/hero/absorb/left/";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case grab:
		animationLength = 12;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/grab/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/grab/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/grab/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/grab/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		spriteName = "Game/worldSprites/hero/move/down/";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case combatState:
		animationLength = 5;
		switch (side)
		{
			case up:
				spriteName = "Game/worldSprites/hero/move/up/";
			case right:
				spriteName = "Game/worldSprites/hero/move/up/";
			case down:
				spriteName = "Game/worldSprites/hero/move/up/";
			case left:
				spriteName = "Game/worldSprites/hero/move/up/";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionDown:
		animationLength = 18;
		if (direction != STAND)
		{
			switch (direction)
			{
				case LEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case RIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case UP:
				{
					spriteName = "Game/worldSprites/hero/move/up/";
					break;
				}
				case DOWN:
				{
					spriteName = "Game/worldSprites/hero/move/down/";
					break;
				}
				case UPLEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case UPRIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case DOWNLEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case DOWNRIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			switch (side)
			{
				case up:
				{
					spriteName = "Game/worldSprites/hero/move/up/";
					break;
				}
				case right:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case down:
				{
					spriteName = "Game/worldSprites/hero/move/down/";
					break;
				}
				case left:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				default:
				{
					break;
				}
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionUp:
		animationLength = 18;
		if (direction != STAND)
		{
			switch (direction)
			{
				case LEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case RIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case UP:
				{
					spriteName = "Game/worldSprites/hero/move/up/";
					break;
				}
				case DOWN:
				{
					spriteName = "Game/worldSprites/hero/move/down/";
					break;
				}
				case UPLEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case UPRIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case DOWNLEFT:
				{
					spriteName = "Game/worldSprites/hero/move/left/";
					break;
				}
				case DOWNRIGHT:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			switch (side)
			{
				case up:
				{
					spriteName = "Game/worldSprites/hero/move/up/";
					break;
				}
				case right:
				{
					spriteName = "Game/worldSprites/hero/move/right/";
					break;
				}
				case down:
				{
					spriteName = "Game/worldSprites/hero/move/down/";
					break;
				}
				case left:
				{
					spriteName = "Game/worldSprites/hero/move/left/"; 
					break;
				}
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case openInventory:
		animationLength = 12;
		spriteName = "Game/worldSprites/hero/grab/down/";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case relax:
		animationLength = 1;
		currentSprite = 1;
		switch (side)
		{
			case up:
			{
				spriteName = "Game/worldSprites/hero/stand/up/";
				break;
			}
			case right:
			{
				spriteName = "Game/worldSprites/hero/stand/right/";
				break;
			}
			case down:
			{
				spriteName = "Game/worldSprites/hero/stand/down/";
				break;
			}
			case left:
			{
				spriteName = "Game/worldSprites/hero/stand/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}

	if (currentAction == move)
	{
		animationLength = 18;
		switch (direction)
		{
		case LEFT:
			spriteName = "Game/worldSprites/hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			
			break;
		case RIGHT:
			spriteName = "Game/worldSprites/hero/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "Game/worldSprites/hero/move/up/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "Game/worldSprites/hero/move/down/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "Game/worldSprites/hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPRIGHT:
			spriteName = "Game/worldSprites/hero/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNLEFT:
			spriteName = "Game/worldSprites/hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNRIGHT:
			spriteName = "Game/worldSprites/hero/move/right/";
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
			lastAction = currentAction;
			currentSprite = 1;
		}
		else
		{
			lastAction = relax;
		}
	}

	return spriteName;
}
