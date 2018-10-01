#include "Helper.h"
#include "World.h"
#include "MenuMaker.h"
#include "ButtonMaker.h"
#include <cmath>
#include <thread>
#include <vector>
#include <array>

using namespace sf;
using namespace std;

int main() {	
	srand(time(0));
	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	MenuMaker menuSystem;
	World world(40000, 40000);
	world.initLightSystem(mainWindow);
	bool windowFocus = true;

	Clock interactClock;
	Clock drawClock;
	Clock scaleDecreaseClock;	

	float interactTime = 0, drawTime = 0;

	Sprite test;
	Texture testTexture;
	testTexture.loadFromFile("Game/worldSprites/hero/stand/down/1.png");
	test.setTexture(testTexture);
	test.setPosition(Vector2f(0, 0));

	int currentMouseButton = 0;

	while (mainWindow.isOpen())
	{
		Event event;
		
		while (mainWindow.pollEvent(event))
		{					
			if (Mouse::isButtonPressed(Mouse::Left))
				currentMouseButton = 1;
			else
				if (Mouse::isButtonPressed(Mouse::Right))
					currentMouseButton = 2;

			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (menuSystem.getState() == closed)
					world.setScaleFactor(event.mouseWheel.delta);
			}	

			if (event.type == Event::MouseButtonReleased)
			{			
				if (menuSystem.getState() == closed)
					world.onMouseDownInteract(currentMouseButton);
					
				if (currentMouseButton == 1)
					menuSystem.interact(world, mainWindow);
			}

			if (event.type == Event::KeyReleased)
			{
				menuSystem.onKeyDown(event, world);
			}

			if (event.type == Event::GainedFocus)			
				windowFocus = true;	

			if (event.type == Event::LostFocus)			
				windowFocus = false;			

			if (event.type == Event::Closed)
			{
				world.Save();
				world.~World();
				mainWindow.close();
				break;
			}
		}		

		if (menuSystem.getState() == mainMenu)
		{		
			mainWindow.clear(Color::White);
			menuSystem.drawButtons(mainWindow);
			mainWindow.display();			
			continue;
		}	 	
			
		if (windowFocus && menuSystem.getState() != gameMenu)
		{
			interactTime = interactClock.getElapsedTime().asMicroseconds();
			interactClock.restart();

			drawTime = drawClock.getElapsedTime().asMicroseconds();
			drawClock.restart();

			world.focusedObject->handleInput();
			world.interact(mainWindow, interactTime);

			mainWindow.clear(Color::White);

			world.draw(mainWindow, drawTime);
		}	
		else
		{
			world.draw(mainWindow, 0);
			menuSystem.drawButtons(mainWindow);
			interactClock.restart();
			drawClock.restart();
		}

		auto hero = dynamic_cast<Deerchant*>(world.focusedObject);

		if (hero->getHealthPoint() <= 0)
		{
			menuSystem.setState(mainMenu);
		}

		RectangleShape healthRect(Vector2f(int(hero->getHealthPoint() / hero->getMaxHealthPointValue() * screenSize.x / 4), screenSize.y * 1 / 20));
		healthRect.setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 8, screenSize.y * 9 / 10));
		healthRect.setFillColor(Color(184, 37, 37));
		mainWindow.draw(healthRect);

		//Helper::drawText(to_string(world.mainScale * 1.5), 30, 200, 300, &mainWindow);
		Helper::drawText(to_string(interactTime), 30, 200, 300, &mainWindow);

		mainWindow.display();
	}
}






