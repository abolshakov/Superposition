#include "Helper.h"
#include "World.h"
#include "MenuMaker.h"
#include "ButtonMaker.h"
#include "HeroBook.h"
#include "TextWriter.h"

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
	int currentMouseButton = 0;

	HeroBook mainBook;

	TextWriter textWriter;

	//Sprite test; Texture testT; testT.loadFromFile("Game/Shaders/fog/fog (1).png"); test.setTexture(testT); test.setPosition(Vector2f(0, 0)); 
	//test.setScale(Helper::GetScreenSize().x / testT.getSize().x, Helper::GetScreenSize().y / testT.getSize().y);

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
				if (menuSystem.getState() == closed && world.getBuildSystem().succesInit)
				{
					world.onMouseDownInteract(currentMouseButton);
					mainBook.onMouseUp();
				}
					
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

			interactClock.restart();
			drawClock.restart();
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
			mainBook.boundHeldItem(&world.getInventorySystem().getHeldItem());
			mainBook.interact(interactTime);

			mainWindow.clear(Color::White);

			world.draw(mainWindow, drawTime);
			world.runBuildSystemDrawing(mainWindow, drawTime);
			mainBook.draw(&mainWindow, world.focusedObject->getHealthPoint() / world.focusedObject->getMaxHealthPointValue(), drawTime);
			world.runInventorySystemDrawing(mainWindow, drawTime);
		}
		else
		{
			world.draw(mainWindow, 0);
			interactClock.restart();
			drawClock.restart();
		}

		menuSystem.drawButtons(mainWindow);

		auto hero = dynamic_cast<DynamicObject*>(world.focusedObject);

		textWriter.drawString(mainBook.debugInfo, NormalFont, 30, 500, 500, &mainWindow);

		/*if (hero->getHealthPoint() <= 0)
		{
			menuSystem.setState(mainMenu);
		}*/

		/*Sprite testS;
		Texture testT;
		for (int i = 0; i < 100; i++)
		{
			testT.loadFromFile("Game/heroBook/Sprites/nightmareBlock.png");
			testS.setTexture(testT);
			mainWindow.draw(testS);
		}*/

		textWriter.drawString(std::to_string(10e5 / drawTime), NormalFont, 30, 200, 200, &mainWindow, Color::Black);
		/*const int groundIndX = hero->getPosition().x / 1000;
		const int groundIndY = hero->getPosition().y / 1000;
		textWriter.drawString(std::to_string(world.biomeMatrix[groundIndX][groundIndY].biomeCell), NormalFont, 30, 200, 200, &mainWindow, Color::Black);*/
		mainWindow.display();
	}
}






