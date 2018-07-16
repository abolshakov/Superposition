#include "Helper.h"
#include "World.h"
#include "Menu.h"
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
	
	Menu menu;
	World world(40000, 40000);
	world.initLightSystem(mainWindow);
	bool windowFocus = true;

	Clock interactClock;
	Clock drawClock;
	Clock scaleDecreaseClock;	

	float interactTime = 0, drawTime = 0;

	while (mainWindow.isOpen())
	{
		Event event;
		
		while (mainWindow.pollEvent(event))
		{						
			if (event.type == sf::Event::MouseWheelMoved)
			{
				world.setScaleFactor(event.mouseWheel.delta);
			}	
			if (event.type == Event::MouseButtonPressed)
			{
				if (world.getBuildSystem().succesInit)
					world.getBuildSystem().onMouseDownInteract(world.focusedObject->getPosition(), world.scaleFactor);
			}
			if (event.type == Event::MouseButtonReleased)
			{
				world.onMouseDownInteract();
			}
			if (event.type == Event::GainedFocus)
			{
				windowFocus = true;
			}
			if (event.type == Event::LostFocus)
			{
				windowFocus = false;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				if (menu.isMenu)
					mainWindow.close();
				else
				{
					world.Save();					
					menu.isMenu = true;
				}
			}
			if (event.type == Event::Closed)
			{
				world.Save();
				world.~World();
				mainWindow.close();
				break;
			}
		}		

		if (menu.isMenu)
		{		
			mainWindow.clear(Color::White);
			menu.interact(world, mainWindow);			
			mainWindow.display();			
			continue;
		}	 	
			
		if (windowFocus)
		{
			interactTime = interactClock.getElapsedTime().asMicroseconds();
			interactClock.restart();

			drawTime = drawClock.getElapsedTime().asMicroseconds();
			drawClock.restart();
		}

		world.focusedObject->handleInput();
		world.interact(mainWindow, interactTime);

			mainWindow.clear(Color::White);
				
		world.draw(mainWindow, drawTime);

		auto hero = dynamic_cast<Deerchant*>(world.focusedObject);

		if (hero->getHealthPoint() <= 0)
		{
			menu.isMenu = true;
		}

		RectangleShape energyRect(Vector2f(int(hero->getEnergy() / hero->getMaxEnergyValue() * screenSize.x / 4), 40));
		energyRect.setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 8, screenSize.y - 100));
		energyRect.setFillColor(Color(245, 215, 66));
		mainWindow.draw(energyRect);
		RectangleShape healthRect(Vector2f(int(hero->getHealthPoint() / hero->getMaxHealthPointValue() * screenSize.x / 4), 40));
		healthRect.setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 8, screenSize.y - 200));
		healthRect.setFillColor(Color(184, 37, 37));
		mainWindow.draw(healthRect);

		//Helper::drawText(to_string(drawTime), 30, 200, 200, &mainWindow);	
		//Helper::drawText(to_string(world.focusedObject->getPosition().x), 30, 200, 300, &mainWindow);
		//Helper::drawText(to_string(world.focusedObject->getPosition().y), 30, 200, 400, &mainWindow);


		mainWindow.display();
	}
}






