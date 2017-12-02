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
	healthPoint = 10000000;
	energy = 50; maxEnergyValue = 100; energyForSpecial = 20;
	currentAction = relax;
	currentWorld = "common";
}

Vector2i Deerchant::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, textureBox.height * 7 / 8);
}

Vector2f Deerchant::getScaleRatio()
{
	return Vector2f(Helper::GetScreenSize().x / (originalTextureBox.width * 10), Helper::GetScreenSize().y / (originalTextureBox.height * 5));
}


void Deerchant::handleInput()
{	
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

void Deerchant::doKick(std::vector<DynamicObject>& dynamicItems, Vector2i mousePos, float scaleFactor)
{
	for (DynamicObject& victim : dynamicItems)
	{
		Vector2f victimPos = Vector2f((victim.getPosition().x - position.x)*scaleFactor + Helper::GetScreenSize().x / 2, (victim.getPosition().y - position.y)*scaleFactor + Helper::GetScreenSize().y / 2);
		if (sqrt(pow(position.x - victimPos.x, 2) + pow(position.y - victimPos.y, 2)) <= this->radius + victim.radius)
		{
			victim.takeDamage(this->strength);
		}
	}
}

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
			spriteName = "heroL_";
		case right:
			spriteName = "heroL_";
		case down:
			spriteName = "heroL_";
		case left:
			spriteName = "heroL_";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";		
		break;
	case hardHit:
		animationLength = 8;
		switch (hitDirection)
		{
		case up:
			spriteName = "heroR_";
		case right:
			spriteName = "heroR_";
		case down:
			spriteName = "heroR_";
		case left:
			spriteName = "heroR_";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case specialHit:
		animationLength = 6;
		switch (hitDirection)
		{
		case up:
			spriteName = "heroF_";
		case right:
			spriteName = "heroF_";
		case down:
			spriteName = "heroF_";
		case left:
			spriteName = "heroF_";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case transitionToEnotherWorld:
		animationLength = 6;
		spriteName = "heroF_";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case combatState:
		animationLength = 5;
		switch (hitDirection)
		{
		case up:
			spriteName = "heroB_";
		case right:
			spriteName = "heroB_";
		case down:
			spriteName = "heroB_";
		case left:
			spriteName = "heroB_";
		}
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionDown:
		animationLength = 5;
		spriteName = "heroF_";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case evasionUp:
		animationLength = 5;
		spriteName = "heroF_";
		spriteName += std::to_string(currentSprite);
		spriteName += ".png";
		break;
	case relax:
		animationLength = 1;
		spriteName = "heroF_0.png";
		currentSprite = 0;
		break;
	}

	if (currentAction == move)
	{
		animationLength = 8;
		switch (direction)
		{
		case LEFT:
			spriteName = "heroL_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case RIGHT:
			spriteName = "heroR_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UP:
			spriteName = "heroB_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case DOWN:
			spriteName = "heroF_";
			spriteName += std::to_string(currentSprite);
			spriteName += ".png";
			break;
		case UPLEFT:
			spriteName = "heroBL_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case UPRIGHT:
			spriteName = "heroBR_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case DOWNLEFT:
			spriteName = "heroFL_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
			break;
		case DOWNRIGHT:
			spriteName = "heroFR_";
			//spriteName += to_string(currentSprite);
			spriteName += "0.png";
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
			currentSprite = 1;
		}
	}

	return spriteName;
}