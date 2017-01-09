#include "Deerchant.h"
#include "Helper.h"

using namespace sf;

Deerchant::Deerchant(Vector2f position, Vector2u size, std::string name, std::string type) : DynamicObject(position, size, name, type)
{		
	currentImg = 1;
	lastTime = 0, timeForNewImage = 0;
	speed = 10;
	imgInRunAnimation = 8;
	bodyElementsNumber = 0;
}

void Deerchant::move(float time)
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
		speed += 1;
	if (Keyboard::isKeyPressed(Keyboard::LShift) && speed > 1)
		speed -= 1;
	sqrtspeed = ceil(speed / float(sqrt(2)));

	if (direction == LEFT)
	{
		position.x -= speed;
		name = "heroL_";
		name += std::to_string(currentImg);
		name += ".png";

	}
	else
		if (direction == RIGHT)
		{
			position.x += speed;
			name = "heroR_";
			name += std::to_string(currentImg);
			name += ".png";
		}
		else
			if (direction == UP)
			{
				position.y -= speed;
				name = "heroB_";
				name += std::to_string(currentImg);
				name += ".png";
			}
			else
				if (direction == DOWN)
				{
					position.y += speed;
					name = "heroF_";
					name += std::to_string(currentImg);
					name += ".png";
				}
				else
					if (direction == UPLEFT)
					{
						position.x -= sqrtspeed;
						position.y -= sqrtspeed;
						name = "heroBL_";
						//name += to_string(currentImg);
						name += "0.png";
					}
					else
						if (direction == UPRIGHT)
						{
							position.x += sqrtspeed;
							position.y -= sqrtspeed;
							name = "heroBR_";
							//name += to_string(currentImg);
							name += "0.png";
						}
						else
							if (direction == DOWNLEFT)
							{
								position.x -= sqrtspeed;
								position.y += sqrtspeed;
								name = "heroFL_";
								//name += to_string(currentImg);
								name += "0.png";
							}
							else
								if (direction == DOWNRIGHT)
								{
									position.x += sqrtspeed;
									position.y += sqrtspeed;
									name = "heroFR_";
									//name += to_string(currentImg);
									name += "0.png";
								}
								else
								{
									if (direction == STAND)
									{
										name = "heroF_0.png";
										currentImg = 0;
										//name += ".png";
									}
								}

	if (timeForNewImage > time)
	{
		timeForNewImage = 0;
	}
	if (time - timeForNewImage >= 500000/speed && direction != STAND)
	{
		timeForNewImage = time;
		currentImg++;
		if (currentImg > imgInRunAnimation)
		{
			currentImg = 1;
		}
	}
}

