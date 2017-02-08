#include "Helper.h"
#include "World.h"

using namespace sf;

int main() {

	World world(100000, 100000);
	world.generate(15000);
	float scaleDecrease = 0;
	float timeForScaleDecrease = 0;

	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	Clock interactClock;
	Clock drawClock;
	Clock scaleDecreaseClock;

	while (mainWindow.isOpen())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				mainWindow.close();
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta == -1 && world.scaleFactor > 0.5)
				{
					world.scaleFactor -= 0.01;
					scaleDecrease = -0.04;
				}
				else
					if (event.mouseWheel.delta == 1 && world.scaleFactor < 1.5)
					{
						world.scaleFactor += 0.01;
						scaleDecrease = 0.04;
					}
			}
		}
		if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
		{
			if (world.scaleFactor != 0.5 && world.scaleFactor != 1.5)
			world.scaleFactor += scaleDecrease;
			if (scaleDecrease < 0 && world.scaleFactor < 0.5)
				world.scaleFactor = 0.5;
			if (scaleDecrease > 0 && world.scaleFactor > 1.5)
				world.scaleFactor = 1.5;

			if (scaleDecrease < 0)
			{
				scaleDecrease += 0.0025;		
			}
			else
				if (scaleDecrease > 0)
				{
					scaleDecrease -= 0.0025;
				}
			
			
			timeForScaleDecrease = 0;
		}
		world.focusedObject->handleInput();
		auto interactTime = interactClock.getElapsedTime().asMicroseconds();
		timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
		scaleDecreaseClock.restart();
		interactClock.restart();
		world.interact(mainWindow, interactTime);

		auto drawTime = drawClock.getElapsedTime().asMicroseconds();
		drawClock.restart();
		mainWindow.clear(Color::White);
		Helper::drawText(std::to_string(world.scaleFactor), 50, 200, 200, &mainWindow);
		world.draw(mainWindow, drawTime);
		mainWindow.display();
	}
}