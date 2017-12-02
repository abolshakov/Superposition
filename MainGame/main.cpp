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

class Map {
public:
	Map(RenderWindow& window, World& world)
	{
		frameTexture.loadFromFile("World/Map/mapBackground.png");
		frame.setTexture(frameTexture);
		frame.setPosition(window.getSize().x - world.getWorldSize().x/100, 0);
	}
	void draw(RenderWindow& window, World& world)
	{
		window.draw(frame);		

		for (auto item : world.staticItems)
		{
			//Helper::drawText(item->getName(), 50, 100, 100, &window);
			if (item->getName()[0] != 'g')
			{
				Sprite sprite = world.spriteMap["Map/" + Helper::getSpriteName(item->getName())].sprite;
				sprite.setPosition(item->getPosition().x / 100 + frame.getPosition().x, item->getPosition().y / 100);
				window.draw(sprite);
			}
		}
	}
private:
	Sprite frame;
	Texture frameTexture;
};

int main() {		
	Menu menu;
	World world(40000, 40000);
	Builder builder;	
	float scaleDecrease = 0;
	float timeForScaleDecrease = 0;
	float pi = 3.14159265358979323846;
	bool isGenerate = false;	

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	Map  map(mainWindow, world);

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
		
		/*Helper::drawText(to_string(hero->isBuilder), 50, 200, 500, &mainWindow);
		Helper::drawText(to_string(hero->getTextureBoxSize().x), 50, 100, 100, &mainWindow);
		Helper::drawText(to_string(interactTime), 50, 100, 200, &mainWindow);
		Helper::drawText(to_string(hero->getPosition().x - world.getBossSpawnPosition().x), 50, 100, 300, &mainWindow);
		Helper::drawText(to_string(hero->isBuilder), 50, 200, 500, &mainWindow);*/

		if (hero->getHealthPoint() <= 0)
		{
			menu.isMenu = true;
		}

		RectangleShape energyRect(Vector2f(int(hero->getEnergy() / hero->getMaxEnergyValue() * 500), 40));
		energyRect.setPosition(Vector2f(screenSize.x / 2 - 250, screenSize.y - 100));
		energyRect.setFillColor(Color::Yellow);
		mainWindow.draw(energyRect);
		//map.draw(mainWindow, world);
		//Helper::drawText(to_string(Helper::getSpriteName(item->getName())))
		
		builder.draw(mainWindow, world, interactTime);

		/*for (int ci = 3000; ci <= 4000; ci+= 10)
		{
			for (int cj = 3000; cj <= 4000; cj+= 10)
			{
				RectangleShape req(Vector2f(5, 5));				
				auto item = world.staticGrid.getItemByName("testTree");
				auto tree = dynamic_cast<TerrainObject*>(item);
				if (!tree)
					continue;
				Vector2i f1 = tree->getFocus1();
				Vector2i f2 = tree->getFocus2();
				if (sqrt((ci - f1.x)*(ci - f1.x) + (cj - f1.y)*(cj - f1.y)) + sqrt((ci - f2.x)*(ci - f2.x) + (cj - f2.y)*(cj - f2.y)) <= tree->getEllipseSize())
				{
					req.setPosition((ci - world.dynamicGrid.getItemByName("hero")->getPosition().x)*world.scaleFactor + screenSize.x/2, (cj - world.dynamicGrid.getItemByName("hero")->getPosition().y)*world.scaleFactor + screenSize.y/2);
					req.setFillColor(Color::Red);
					mainWindow.draw(req);
				}
			}
		}*/		
		
		mainWindow.display();
	}
}