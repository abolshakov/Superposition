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

	world.scaleFactor = 1;

	Sprite test;
	Texture testTexture;
	testTexture.loadFromFile("World/Hero/stand/down/1.png");
	test.setTexture(testTexture);
	test.setPosition(Vector2f(0, 0));

	while (mainWindow.isOpen())
	{
		Event event;
		
		while (mainWindow.pollEvent(event))
		{						
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (menuSystem.getState() == closed)
					world.setScaleFactor(event.mouseWheel.delta);
			}	

			if (event.type == Event::MouseButtonReleased)
			{										
				if (menuSystem.getState() == closed)
					world.onMouseDownInteract();

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

		RectangleShape energyRect(Vector2f(int(hero->getEnergy() / hero->getMaxEnergyValue() * screenSize.x / 4), 40));
		energyRect.setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 8, screenSize.y - 100));
		energyRect.setFillColor(Color(245, 215, 66));
		mainWindow.draw(energyRect);
		RectangleShape healthRect(Vector2f(int(hero->getHealthPoint() / hero->getMaxHealthPointValue() * screenSize.x / 4), 40));
		healthRect.setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 8, screenSize.y - 200));
		healthRect.setFillColor(Color(184, 37, 37));
		mainWindow.draw(healthRect);

		//Helper::drawText(world.focusedObject->getSpriteName(drawTime), 30, 200, 300, &mainWindow);
		//Helper::drawText(to_string(world.focusedObject->getPosition().y), 30, 200, 400, &mainWindow);		

		mainWindow.display();
	}
}






