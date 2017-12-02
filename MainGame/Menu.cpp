#include "Menu.h"

Menu::Menu()
{
	screenSize = Helper::GetScreenSize();
	loadingImageTexture.loadFromFile("World/Loading.png");
	loadingImage.setTexture(loadingImageTexture);
	loadingImage.setPosition(screenSize.x, screenSize.y);

	newRunButtonPosition = Vector2f(screenSize.x / 16, screenSize.y / 8);
	exitButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 5 / 8);
	settingsButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 3 / 8);
	continueButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 2 / 8);

	newRunButtonTexture.loadFromFile("World/newRun.png");
	newRunButton.setTexture(newRunButtonTexture);
	exitButtonTexture.loadFromFile("World/exit.png");
	exitButton.setTexture(exitButtonTexture);
	settingsButtonTexture.loadFromFile("World/settings.png");
	settingsButton.setTexture(settingsButtonTexture);
	continueButtonTexture.loadFromFile("World/continue.png");
	continueButton.setTexture(continueButtonTexture);

	newRunButton.setPosition(newRunButtonPosition);
	exitButton.setPosition(exitButtonPosition);
	settingsButton.setPosition(settingsButtonPosition);
	continueButton.setPosition(continueButtonPosition);
}

void Menu::drawLoadingImage(RenderWindow &window)
{
	window.draw(loadingImage);
}

void Menu::interact(World &world, RenderWindow &window)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (Helper::isIntersects(mousePos, IntRect(newRunButtonPosition.x, newRunButtonPosition.y, newRunButtonTexture.getSize().x, newRunButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			//Thread thread(Menu::drawLoadingImage, &window);				
			//thread.launch();
			world.generate(3000);
			isMenu = false;
		}
		newRunButton.setColor(Color(newRunButton.getColor().r, newRunButton.getColor().g, newRunButton.getColor().b, 128));
	}
	else
		newRunButton.setColor(Color(newRunButton.getColor().r, newRunButton.getColor().g, newRunButton.getColor().b, 255));

	if (Helper::isIntersects(mousePos, IntRect(continueButtonPosition.x, continueButtonPosition.y, continueButtonTexture.getSize().x, continueButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			//Thread thread(Menu::drawLoadingImage, &window);				
			//thread.launch();
			world.Load();
			isMenu = false;
		}
		continueButton.setColor(Color(continueButton.getColor().r, continueButton.getColor().g, continueButton.getColor().b, 128));
	}
	else
		continueButton.setColor(Color(continueButton.getColor().r, continueButton.getColor().g, continueButton.getColor().b, 255));

	if (Helper::isIntersects(mousePos, IntRect(settingsButtonPosition.x, settingsButtonPosition.y, settingsButtonTexture.getSize().x, settingsButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{

		}
		settingsButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 128));
	}
	else
		settingsButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 250));

	if (Helper::isIntersects(mousePos, IntRect(exitButtonPosition.x, exitButtonPosition.y, exitButtonTexture.getSize().x, exitButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			isMenu = false;
			window.close();
		}
		exitButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 128));
	}
	else
		exitButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 255));

	drawButtons(window);
}

void Menu::drawButtons(RenderWindow &window)
{
	if (newRunButtonShow)
		window.draw(newRunButton);
	if (exitButtonShow)
		window.draw(exitButton);
	if (settingsButtonShow)
		window.draw(settingsButton);
	if (continueButtonShow)
		window.draw(continueButton);
}
	
