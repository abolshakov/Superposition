#include "Deerchant.h"
#include "Helper.h"

using namespace sf;

Deerchant::Deerchant(std::string objectName, Vector2f centerPosition) : DynamicObject(objectName, centerPosition)
{
	currentSprite = 1;
	timeForNewSprite = 0;
	speed = 0.0005f;
	animationLength = 8;
}

Vector2f Deerchant::calculateTextureOffset()
{
	return Vector2f(textureBox.width / 2, textureBox.height * 7 / 8);
}

void Deerchant::handleInput()
{
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
									direction = STAND;

	if (Keyboard::isKeyPressed(Keyboard::LControl))
		speed /= 2;
	else if (Keyboard::isKeyPressed(Keyboard::LShift) && speed > 1)
		speed *= 2;
}

const std::string Deerchant::getSpriteName(long long elapsedTime)
{
	if (direction != STAND)
	{
		timeForNewSprite += elapsedTime;

		if (timeForNewSprite >= 150000000 * speed)
		{
			timeForNewSprite = 0;
			
			if (++currentSprite > animationLength)
				currentSprite = 1;
		}
	}
	std::string spriteName;

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
	case STAND:
		spriteName = "heroF_0.png";
		currentSprite = 0;
	default:;
	}
	return spriteName;
}