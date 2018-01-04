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
	inventory.resize(8);
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
	if (Mouse::isButtonPressed(Mouse::Left) && (currentAction == relax || currentAction == combatState))
	{
		setHitDirection();
		currentAction = commonHit;
		currentSprite = 1;
	}
	else
	if (Mouse::isButtonPressed(Mouse::Right) && (currentAction == relax || currentAction == combatState))
	{
		setHitDirection();
		currentAction = hardHit;
		currentSprite = 1;
	}
	else
	if (Keyboard::isKeyPressed(Keyboard::F) && (currentAction == relax || currentAction == combatState) && energy >= energyForSpecial)
	{
		energy -= energyForSpecial;
		setHitDirection();
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
		currentAction = evasionDown;
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

/*void Deerchant::doKick(std::vector<DynamicObject>& dynamicItems, Vector2i mousePos, float scaleFactor)
{
	for (DynamicObject& victim : dynamicItems)
	{
		Vector2f victimPos = Vector2f((victim.getPosition().x - position.x)*scaleFactor + Helper::GetScreenSize().x / 2, (victim.getPosition().y - position.y)*scaleFactor + Helper::GetScreenSize().y / 2);
		if (sqrt(pow(position.x - victimPos.x, 2) + pow(position.y - victimPos.y, 2)) <= this->radius + victim.radius)
		{
			if (victim.currentAction != dead)
				victim.takeDamage(this->strength);
			else
				victim.isVisibleInventory = true;
		}
	}
}*/

std::string Deerchant::getSpriteName(long long elapsedTime)
{
	std::string spriteName;

	switch (currentAction)
	{
	case commonHit:
		animationLength = 5;
		switch (hitDirection)
		{
		case up: 
			spriteName = "Maddox/MaddoxLeft";
		case right:
			spriteName = "Maddox/MaddoxLeft";
		case down:
			spriteName = "Maddox/MaddoxLeft";
		case left:
			spriteName = "Maddox/MaddoxLeft";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";		
		break;
	case hardHit:
		animationLength = 8;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/MaddoxRight";
		case right:
			spriteName = "Maddox/MaddoxRight";
		case down:
			spriteName = "Maddox/MaddoxRight";
		case left:
			spriteName = "Maddox/MaddoxRight";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case specialHit:
		animationLength = 6;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/MaddoxFront";
		case right:
			spriteName = "Maddox/MaddoxFront";
		case down:
			spriteName = "Maddox/MaddoxFront";
		case left:
			spriteName = "Maddox/MaddoxFront";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case transitionToEnotherWorld:
		animationLength = 6;
		spriteName = "Maddox/MaddoxFront";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case combatState:
		animationLength = 5;
		switch (hitDirection)
		{
		case up:
			spriteName = "Maddox/MaddoxBack";
		case right:
			spriteName = "Maddox/MaddoxBack";
		case down:
			spriteName = "Maddox/MaddoxBack";
		case left:
			spriteName = "Maddox/MaddoxBack";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionDown:
		animationLength = 5;
		spriteName = "Maddox/MaddoxFront";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionUp:
		animationLength = 5;
		spriteName = "Maddox/MaddoxBack";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case openInventory:
		animationLength = 5;
		spriteName = "Maddox/MaddoxBack";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case relax:
		animationLength = 1;
		currentSprite = 1;
		spriteName = "Maddox/Maddox";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	}

	if (currentAction == move)
	{
		switch (direction)
		{
		case LEFT:
			spriteName = "Maddox/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case RIGHT:
			spriteName = "Maddox/MaddoxRight";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case UP:
			spriteName = "Maddox/MaddoxBack";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 8;
			break;
		case DOWN:
			spriteName = "Maddox/MaddoxFront";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 8;
			break;
		case UPLEFT:
			spriteName = "Maddox/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case UPRIGHT:
			spriteName = "Maddox/MaddoxRight";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case DOWNLEFT:
			spriteName = "Maddox/MaddoxLeft";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			animationLength = 10;
			break;
		case DOWNRIGHT:
			spriteName = "Maddox/MaddoxRight";
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
