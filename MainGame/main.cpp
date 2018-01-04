#include "Helper.h"
#include "World.h"
#include "Deerchant.h"
#include "Builder.h"
#include <cmath>
#include <thread>
#include "Menu.h"
#include <vector>
#include <array>

using namespace sf;
using namespace std;

int main() {	
	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	sf::View view = mainWindow.getDefaultView();

	Menu menu;
	World world(40000, 40000);
	Builder builder(world.getInventorySystem().getSpriteList());	
	float scaleDecrease = 0;
	float timeForScaleDecrease = 0;
	float pi = 3.14159265358979323846;
	bool isGenerate = false;	

	world.initLightSystem(mainWindow);

	Clock interactClock;
	Clock drawClock;
	Clock scaleDecreaseClock;	

	while (mainWindow.isOpen())
	{
		Event event;
		
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				world.Save();
				mainWindow.close();
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				if (menu.isMenu)
					mainWindow.close();
				world.Save();
				menu.isMenu = true;				
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				world.setScaleFactor(event.mouseWheel.delta);
			}	
			if (event.type == Event::MouseButtonPressed)
				builder.onMouseDownInteract(world);
		}		
		auto interactTime = interactClock.getElapsedTime().asMicroseconds();
		interactClock.restart();

		auto drawTime = drawClock.getElapsedTime().asMicroseconds();
		drawClock.restart();

		if (menu.isMenu)
		{		
			mainWindow.clear(Color::White);
			menu.interact(world, mainWindow);			
			mainWindow.display();			
			continue;
		}	 	
		
		auto hero = dynamic_cast<Deerchant*>(world.dynamicGrid.getItemByName(world.heroName));
		
		builder.setCharacterBuildActivity(*hero);
		world.focusedObject->handleInput();
		world.interact(mainWindow, interactTime);	
		
		mainWindow.clear(Color::White);
		builder.interact();
				
		world.draw(mainWindow, drawTime);

		if (hero->getHealthPoint() <= 0)
		{
			menu.isMenu = true;
		}

		RectangleShape energyRect(Vector2f(int(hero->getEnergy() / hero->getMaxEnergyValue() * 500), 40));
		energyRect.setPosition(Vector2f(screenSize.x / 2 - 250, screenSize.y - 100));
		energyRect.setFillColor(Color::Yellow);
		mainWindow.draw(energyRect);
		RectangleShape healthRect(Vector2f(int(hero->getHealthPoint() / hero->getMaxHealthPointValue() * 500), 40));
		healthRect.setPosition(Vector2f(screenSize.x / 2 - 250, screenSize.y - 200));
		healthRect.setFillColor(Color::Red);
		mainWindow.draw(healthRect);
		
		world.renderLightSystem(view, mainWindow);
		Helper::drawText(to_string(Actions(hero->currentAction)), 30, 100, 100, &mainWindow);
		//Helper::drawText(to_string(hero->getHealthPoint()), 30, 100, 200, &mainWindow);
		builder.draw(mainWindow, world, interactTime);
		mainWindow.display();
	}
}