#include "Helper.h"
#include "World.h"

using namespace sf;

int main() {

	World world(100000, 100000);
	world.initSpriteMap();
	world.generate(15000);

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	mainWindow.setVerticalSyncEnabled(true);
	
	Clock interactClock;
	Clock drawClock;

	while (mainWindow.isOpen())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				mainWindow.close();
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