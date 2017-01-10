//#include <cmath>
#include "Helper.h"
#include "World.h"

using namespace sf;

//Vector2f f1 = Vector2f(100, 100), f2 = Vector2f(300, 100);
//int aEl = 210;

int main() {

	World world(5000, 5000);
	world.initSpriteMap();
	world.generate(120);

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y)), "game");

	Clock clock;
	long long timeToDraw = 0;
	auto fps = 1000000 / Helper::getFps();

	while (mainWindow.isOpen())
	{
		auto elapsedTime = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				mainWindow.close();
			}
		}
		world.focusedObject->handleInput();
		world.interact(elapsedTime);		
		
		timeToDraw += elapsedTime;
		if (timeToDraw >= fps)
		{
			mainWindow.clear(Color::White);
			world.draw(mainWindow, timeToDraw);
			
			/*if (sqrt((character.getPosition().x - f1.x)*(character.getPosition().x - f1.x) + (character.getPosition().y - f1.y)*(character.getPosition().y - f1.y)) + sqrt((character.getPosition().x - f2.x)*(character.getPosition().x - f2.x) + (character.getPosition().y - f2.y)*(character.getPosition().y - f2.y)) <= aEl)
				Helper::drawText(to_string(12345), 50, 200, 200, &mainWindow);
				Helper::drawText(to_string(character.getPosition().x), 50, 500, 200, &mainWindow);
				Helper::drawText(to_string(character.getPosition().y), 50, 500, 300, &mainWindow);*/
			mainWindow.display();
			timeToDraw = 0;
		}
	}
}