#include "Helper.h"
#include "World.h"

using namespace sf;

int main() {

	World world(5000, 5000);
	world.initSpriteMap();
	world.generate(20);

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y)), "game");

	Clock interactClock;
	Clock drawClock;

	while (mainWindow.isOpen())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				mainWindow.close();
			}
		}
		world.focusedObject->handleInput();

		auto interactTime = interactClock.getElapsedTime().asMicroseconds();
		interactClock.restart();
		world.interact(mainWindow, interactTime);

		auto drawTime = drawClock.getElapsedTime().asMicroseconds();
		drawClock.restart();
		mainWindow.clear(Color::White);
		world.draw(mainWindow, drawTime);
		mainWindow.display();
	}
}