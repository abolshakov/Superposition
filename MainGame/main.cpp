#include <cmath>
#include "Helper.h"
#include "Deerchant.h"
#include "World.h"

using namespace sf;
using namespace std;

Vector2f f1 = Vector2f(100, 100), f2 = Vector2f(300, 100);
int aEl = 210;

int main() {
	string test;

	Vector2f screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(sf::VideoMode(screenSize.x, screenSize.y), "game");
	
	Clock clock;
	float time = 0, time2 = 0;
	int fps = 1000000 / Helper::getFps();

	World testWorld(50000, 50000);
	testWorld.initSpriteMap();
	testWorld.worldGenerate(1200);

	Vector2f heroPosition = Vector2f(100, 100);
	Deerchant character(heroPosition, testWorld.spriteMap["heroF_0.png"].texture.getSize(), "heroF_0.png", "character");
	testWorld.dynamicObjects.addItem(character, heroPosition.x, heroPosition.y);

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

			character.move(time);
			if (sqrt((character.getPosition().x - f1.x)*(character.getPosition().x - f1.x) + (character.getPosition().y - f1.y)*(character.getPosition().y - f1.y)) + sqrt((character.getPosition().x - f2.x)*(character.getPosition().x - f2.x) + (character.getPosition().y - f2.y)*(character.getPosition().y - f2.y)) <= aEl)
				Helper::drawText(to_string(12345), 50, 200, 200, &mainWindow);
			Helper::drawText(to_string(character.getPosition().x), 50, 500, 200, &mainWindow);
			Helper::drawText(to_string(character.getPosition().y), 50, 500, 300, &mainWindow);
			mainWindow.display();

			time2 = 0;
		}
	}
}