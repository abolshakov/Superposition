#include "Deerchant.h"
#include "Helper.h"

using namespace sf;

const
float pi = 3.14159265358979323846;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationLength = 8;
	radius = 100;
	strength = 20;
	maxHealthPointValue = 100;
	healthPoint = maxHealthPointValue;
	energy = 50; maxEnergyValue = 100; energyForSpecial = 20;
	currentAction = relax;
	currentWorld = "common";
	inventory.resize(20);
	for (auto curInvItem = inventory.begin(); curInvItem != inventory.end(); ++curInvItem)
	{
		curInvItem->first = 0;
		curInvItem->second = 0;
	}
	inventoryCapacity = 20;
}

Vector2i Deerchant::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2f(250, 250);
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Deerchant::handleInput()
{	
	setHitDirection();
	if (currentAction == move)
		lastAction = move;
	if (Keyboard::isKeyPressed(Keyboard::E))
	{
		currentAction = openInventory;
	}

	if (currentAction != evasionDown && currentAction != evasionUp)
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::D))
			currentAction = move;
		if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
			direction = UPLEFT;
		else
			if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
				direction = UPRIGHT;
			else
				if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
					direction = DOWNLEFT;
				else
					if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
						direction = DOWNRIGHT;
					else
						if (Keyboard::isKeyPressed(Keyboard::A))
							direction = LEFT;
						else
							if (Keyboard::isKeyPressed(Keyboard::D))
								direction = RIGHT;
							else
								if (Keyboard::isKeyPressed(Keyboard::W))
									direction = UP;
								else
									if (Keyboard::isKeyPressed(Keyboard::S))
										direction = DOWN;
									else
									{
										direction = STAND;
										if (currentAction == move)
											currentAction = relax;
									}
	}
	if (isBuilder)
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
	if (Keyboard::isKeyPressed(Keyboard::LControl) && currentAction != move && currentAction != evasionDown && currentAction != evasionUp)
	{
		currentAction = evasionDown;
		currentSprite = 1;
	}
	else
	if (Keyboard::isKeyPressed(Keyboard::Space) && currentAction != move && currentAction != evasionDown && currentAction != evasionUp)
	{
		currentAction = evasionUp;
		currentSprite = 1;
	}
}

void Deerchant::setHitDirection()
{
	auto screenSize = Helper::GetScreenSize();
	float xPos = screenSize.x / 2, yPos = screenSize.y / 2, mouseX = Mouse::getPosition().x, mouseY = Mouse::getPosition().y;
	float alpha = atan((yPos - mouseY) / (xPos - mouseX)) * 180 / pi;
	if (mouseY <= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
		hitDirection = up;
	else
		if (mouseX >= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
			hitDirection = right;
		else
			if (mouseY >= yPos && abs(alpha) >= 45 && abs(alpha) <= 90)
				hitDirection = down;
			else
				if (mouseX <= xPos && abs(alpha) >= 0 && abs(alpha) <= 45)
					hitDirection = left;
}

std::string Deerchant::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
	case commonHit:
		animationLength = 6;
		switch (hitDirection)
		{
		case up:
		{
			spriteName = "Maddox/commonHit/back/ch1_light_b_";
			break;
		}
		case right:
		{
			spriteName = "Maddox/commonHit/front/ch1_light_";
			break;
		}
		case down:
		{
			spriteName = "Maddox/commonHit/front/ch1_light_";
			break;
		}
		case left:
		{
			spriteName = "Maddox/commonHit/front/ch1_light_";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";		
		break;
	case hardHit:
		animationLength = 8;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/moveRight/MaddoxRight";
		case right:
			spriteName = "Maddox/moveRight/MaddoxRight";
		case down:
			spriteName = "Maddox/moveRight/MaddoxRight";
		case left:
			spriteName = "Maddox/moveRight/MaddoxRight";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case specialHit:
		animationLength = 6;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/moveDown/MaddoxDown";
		case right:
			spriteName = "Maddox/moveDown/MaddoxDown";
		case down:
			spriteName = "Maddox/moveDown/MaddoxDown";
		case left:
			spriteName = "Maddox/moveDown/MaddoxDown";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case transitionToEnotherWorld:
		animationLength = 6;
		spriteName = "Maddox/moveDown/MaddoxDown";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case combatState:
		animationLength = 5;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/moveUp/MaddoxUp";
		case right:
			spriteName = "Maddox/moveUp/MaddoxUp";
		case down:
			spriteName = "Maddox/moveUp/MaddoxUp";
		case left:
			spriteName = "Maddox/moveUp/MaddoxUp";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionDown:
		animationLength = 7;
		switch (hitDirection)
		{
			case up:
			{
				spriteName = "Maddox/evasionDown/back/ch1_dodge_b_";
				break;
			}
			case right:
			{	
				spriteName = "Maddox/evasionDown/front/ch1_dodge_f_";
				break;
			}
			case down:
			{
				spriteName = "Maddox/evasionDown/front/ch1_dodge_f_";
				break;
			}
			case left:
			{
				spriteName = "Maddox/evasionDown/left/ch1_dodge_s_";
				break;
			}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionUp:
		animationLength = 8;
		switch (hitDirection)
		{
		case up:
		{
			spriteName = "Maddox/evasionUp/back/ch1_jump_b_";
			break;
		}
		case right:
		{
			spriteName = "Maddox/evasionUp/front/ch1_jump_f_";
			break;
		}
		case down:
		{
			spriteName = "Maddox/evasionUp/front/ch1_jump_f_";
			break;
		}
		case left:
		{
			spriteName = "Maddox/evasionUp/left/ch1_jump_s_";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case openInventory:
		animationLength = 7;
		spriteName = "Maddox/evasionDown/front/ch1_dodge_f_";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case relax:
		animationLength = 1;
		currentSprite = 1;
		switch (hitDirection)
		{
		case up:
		{
			spriteName = "Maddox/ch1_b_";
			break;
		}
		case right:
		{
			spriteName = "Maddox/ch1_r_";
			break;
		}
		case down:
		{
			spriteName = "Maddox/ch1_f_";
			break;
		}
		case left:
		{
			spriteName = "Maddox/ch1_l_";
			break;
		}
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}

	if (currentAction == move)
	{
		switch (direction)
		{
		case LEFT:
			spriteName = "Maddox/moveLeft/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case RIGHT:
			spriteName = "Maddox/moveRight/MaddoxRight";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case UP:
			spriteName = "Maddox/moveUp/MaddoxUp";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 8;
			break;
		case DOWN:
			spriteName = "Maddox/moveDown/MaddoxDown";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 8;
			break;
		case UPLEFT:
			spriteName = "Maddox/moveLeft/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case UPRIGHT:
			spriteName = "Maddox/moveRight/MaddoxRight";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case DOWNLEFT:
			spriteName = "Maddox/moveLeft/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case DOWNRIGHT:
			spriteName = "Maddox/moveRight/MaddoxRight";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		default:;
		}
	}

	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / speed)
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
			currentSprite = 1;
		}
	}

	return spriteName;
}
