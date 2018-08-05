#include "Deerchant.h"
#include "Helper.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationSpeed = 0.0010f;
	animationLength = 8;
	radius = 100;
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
}

Deerchant::~Deerchant()
{

}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2f(500, 500);
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 11 / 16);
}

void Deerchant::handleInput()
{
	setHitDirection();
	moveOffset = Vector2f(-1, -1);

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
		target = nullptr;
	
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
									bool isIntersect;
									if (target)
										isIntersect = (sqrt(pow(this->position.x - targetPosition.x, 2) + pow(this->position.y - targetPosition.y, 2)) <= (this->radius + target->getRadius()));

									if (isIntersect || !target)
									{
										direction = STAND;
										if (currentAction == move)
											currentAction = relax;
									}
									else									
									{
										if (target)
										{
											moveToTarget(target->getRadius());
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
	if (Mouse::isButtonPressed(Mouse::Left) && (currentAction == relax || currentAction == combatState) && lastAction != openInventory)
	{
		currentAction = commonHit;
		currentSprite = 1;
	}
	else
	if (Mouse::isButtonPressed(Mouse::Right) && (currentAction == relax || currentAction == combatState))
	{
		currentAction = hardHit;
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

	if (!(currentAction == relax && lastAction == openInventory))
		lastAction = currentAction;
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

void Deerchant::behavior(DynamicObject& target, float elapsedTime)
{
	bool isIntersect = (sqrt(pow(this->position.x - target.getPosition().x, 2) + pow(this->position.y - target.getPosition().y, 2)) <= (this->radius + target.getRadius()));

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

void Deerchant::onMouseDownBehavior(WorldObject *object)
{
	if (!object)
		return;

	target = object;
	targetPosition = object->getPosition();
	bool isIntersect = (sqrt(pow(this->position.x - object->getPosition().x, 2) + pow(this->position.y - object->getPosition().y, 2)) <= (this->radius + object->getRadius()));

	if (isIntersect)
	{
		switch (object->tag)
		{
		case forestTreeTag:
		{
			//object->delatePromiseOn();
			currentAction = absorbs;
			currentSprite = 1;
			//object->setState(absorbed);
			break;
		}
		case chamomileTag:
		{
			auto item = dynamic_cast<PickedObject*>(object);
			item->pickUp(inventory);
			break;
		}
		}
	}		
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
				spriteName = "Hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Hero/hit/left/";
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
				spriteName = "Hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Hero/hit/left/";
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
				spriteName = "Hero/hit/up/";
				break;
			}
			case right:
			{
				spriteName = "Hero/hit/right/";
				break;
			}
			case down:
			{
				spriteName = "Hero/hit/down/";
				break;
			}
			case left:
			{
				spriteName = "Hero/hit/left/";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case absorbs:
		animationLength = 18;
		switch (side)
		{
		case up:
		{
			spriteName = "Hero/move/up/";
			break;
		}
		case right:
		{
			spriteName = "Hero/move/right/";
			break;
		}
		case down:
		{
			spriteName = "Hero/move/down/";
			break;
		}
		case left:
		{
			spriteName = "Hero/move/left/";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case transitionToEnotherWorld:
		animationLength = 18;
		spriteName = "Hero/move/down/";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case combatState:
		animationLength = 5;
		switch (side)
		{
		case up:
			spriteName = "Hero/move/up/";
		case right:
			spriteName = "Hero/move/up/";
		case down:
			spriteName = "Hero/move/up/";
		case left:
			spriteName = "Hero/move/up/";
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
					spriteName = "Hero/move/left/";
					break;
				}
				case RIGHT:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case UP:
				{
					spriteName = "Hero/move/up/";
					break;
				}
				case DOWN:
				{
					spriteName = "Hero/move/down/";
					break;
				}
				case UPLEFT:
				{
					spriteName = "Hero/move/left/";
					break;
				}
				case UPRIGHT:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case DOWNLEFT:
				{
					spriteName = "Hero/move/left/";
					break;
				}
				case DOWNRIGHT:
				{
					spriteName = "Hero/move/right/";
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
					spriteName = "Hero/move/up/";
					break;
				}
				case right:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case down:
				{
					spriteName = "Hero/move/down/";
					break;
				}
				case left:
				{
					spriteName = "Hero/move/left/";
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
					spriteName = "Hero/move/left/";
					break;
				}
				case RIGHT:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case UP:
				{
					spriteName = "Hero/move/up/";
					break;
				}
				case DOWN:
				{
					spriteName = "Hero/move/down/";
					break;
				}
				case UPLEFT:
				{
					spriteName = "Hero/move/left/";
					break;
				}
				case UPRIGHT:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case DOWNLEFT:
				{
					spriteName = "Hero/move/left/";
					break;
				}
				case DOWNRIGHT:
				{
					spriteName = "Hero/move/right/";
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
					spriteName = "Hero/move/up/";
					break;
				}
				case right:
				{
					spriteName = "Hero/move/right/";
					break;
				}
				case down:
				{
					spriteName = "Hero/move/down/";
					break;
				}
				case left:
				{
					spriteName = "Hero/move/left/"; 
					break;
				}
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case openInventory:
		animationLength = 7;
		spriteName = "Hero/stand/down/";
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
				spriteName = "Hero/stand/up/";
				break;
			}
			case right:
			{
				spriteName = "Hero/stand/right/";
				break;
			}
			case down:
			{
				spriteName = "Hero/stand/down/";
				break;
			}
			case left:
			{
				spriteName = "Hero/stand/left/";
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
			spriteName = "Hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			
			break;
		case RIGHT:
			spriteName = "Hero/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "Hero/move/up/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "Hero/move/down/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "Hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPRIGHT:
			spriteName = "Hero/move/right/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNLEFT:
			spriteName = "Hero/move/left/";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWNRIGHT:
			spriteName = "Hero/move/right/";
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
			if (currentAction == transitionToEnotherWorld)
			{
				if (currentWorld == "common")
					currentWorld = "spirit";
				else
					currentWorld = "common";
				currentAction = relax;
			}
			if (currentAction == commonHit || currentAction == hardHit || currentAction == specialHit || currentAction == evasionDown || currentAction == evasionUp)
			{
				lastAction = relax;
				currentAction = relax;
			}
			if (currentAction == openInventory)
			{
				currentAction = relax;
				lastAction = openInventory;
			}
			if (currentAction == absorbs)
			{
				if (target)
					target->setState(absorbed);
				currentAction = relax;
				lastAction = absorbs;
			}
			currentSprite = 1;
		}
	}

	return spriteName;
}
