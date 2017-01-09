//#include <cmath>
#include "Helper.h"
#include "Deerchant.h"
#include "World.h"

using namespace sf;

Vector2f f1 = Vector2f(100, 100), f2 = Vector2f(300, 100);
int aEl = 210;

int main() {
	std::string test;

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y)), "game");

	Clock clock;
	float time = 0, time2 = 0;
	auto fps = 1000000 / Helper::getFps();

	World testWorld(5000, 5000);
	testWorld.initSpriteMap();
	testWorld.worldGenerate(120);

	auto heroPosition = Vector2f(100, 100);
	std::string name = "hero";
	testWorld.dynamicGrid.addItem(new Deerchant(heroPosition, testWorld.spriteMap["heroF_0.png"].texture.getSize(), "heroF_0.png", "character"), name, int(heroPosition.x), int(heroPosition.y));
	auto character = dynamic_cast<Deerchant*>(testWorld.dynamicGrid.getItemByName(name));
	
	while (mainWindow.isOpen())
	{
		Event event;
		time += clock.getElapsedTime().asMicroseconds();
		time2 += clock.getElapsedTime().asMicroseconds();
		if (time > 10000000)
			time = 0;
		clock.restart();
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				mainWindow.close();
			}
		}

		if (time2 >= fps)
		{
			mainWindow.clear(Color::White);
			testWorld.drawBoard(&mainWindow, screenSize, character);

			character->move(time);
			/*if (sqrt((character.getPosition().x - f1.x)*(character.getPosition().x - f1.x) + (character.getPosition().y - f1.y)*(character.getPosition().y - f1.y)) + sqrt((character.getPosition().x - f2.x)*(character.getPosition().x - f2.x) + (character.getPosition().y - f2.y)*(character.getPosition().y - f2.y)) <= aEl)
				Helper::drawText(to_string(12345), 50, 200, 200, &mainWindow);
				Helper::drawText(to_string(character.getPosition().x), 50, 500, 200, &mainWindow);
				Helper::drawText(to_string(character.getPosition().y), 50, 500, 300, &mainWindow);*/
			mainWindow.display();

			time2 = 0;
		}
	}
}