#include "MenuMaker.h"

MenuMaker::MenuMaker()
{
	screenSize = Helper::GetScreenSize();
	loadingImageTexture.loadFromFile("World/MenuMaker/Loading.png");
	loadingImage.setTexture(loadingImageTexture);
	loadingImage.setPosition(screenSize.x / 2, screenSize.y / 2);

	newRunButtonPosition = Vector2f(screenSize.x / 16, screenSize.y / 8);
	exitButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 5 / 8);
	settingsButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 3 / 8);
	continueButtonPosition = Vector2f(screenSize.x / 16, screenSize.y * 2 / 8);

	newRunButtonTexture.loadFromFile("World/MenuMaker/newRun.png");
	newRunButton.setTexture(newRunButtonTexture);
	exitButtonTexture.loadFromFile("World/MenuMaker/exit.png");
	exitButton.setTexture(exitButtonTexture);
	settingsButtonTexture.loadFromFile("World/MenuMaker/settings.png");
	settingsButton.setTexture(settingsButtonTexture);
	continueButtonTexture.loadFromFile("World/MenuMaker/continue.png");
	continueButton.setTexture(continueButtonTexture);

	newRunButton.setPosition(newRunButtonPosition);
	exitButton.setPosition(exitButtonPosition);
	settingsButton.setPosition(settingsButtonPosition);
	continueButton.setPosition(continueButtonPosition);

	newRunButton.setScale((screenSize.x / 4) / newRunButtonTexture.getSize().x, (screenSize.y / 10) / newRunButtonTexture.getSize().y);
	exitButton.setScale((screenSize.x / 4) / exitButtonTexture.getSize().x, (screenSize.y / 10) / exitButtonTexture.getSize().y);
	settingsButton.setScale((screenSize.x / 4) / settingsButtonTexture.getSize().x, (screenSize.y / 10) / settingsButtonTexture.getSize().y);
	continueButton.setScale((screenSize.x / 4) / continueButtonTexture.getSize().x, (screenSize.y / 10) / continueButtonTexture.getSize().y);
}

MenuMaker::~MenuMaker()
{

}

void MenuMaker::drawLoadingImage(RenderWindow &window)
{
	window.draw(loadingImage);
}

void MenuMaker::interact(World &world, RenderWindow &window)
{
	Vector2f mousePos = (Vector2f)Mouse::getPosition();
	if (Helper::isIntersects(mousePos, IntRect(newRunButtonPosition.x, newRunButtonPosition.y, newRunButtonTexture.getSize().x, newRunButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			//Thread thread(MenuMaker::drawLoadingImage, &window);				
			//thread.launch();
			world.generate(3000);
			//world.initLightSystem(window);
			isMenuMaker = false;
		}
		newRunButton.setColor(Color(newRunButton.getColor().r, newRunButton.getColor().g, newRunButton.getColor().b, 128));
	}
	else
		newRunButton.setColor(Color(newRunButton.getColor().r, newRunButton.getColor().g, newRunButton.getColor().b, 255));

	if (Helper::isIntersects(mousePos, IntRect(continueButtonPosition.x, continueButtonPosition.y, continueButtonTexture.getSize().x, continueButtonTexture.getSize().y)))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			//Thread thread(MenuMaker::drawLoadingImage, &window);				
			//thread.launch();
			world.Load();
			//world.initLightSystem(window);
			isMenuMaker = false;
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
			isMenuMaker = false;
			window.close();
		}
		exitButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 128));
	}
	else
		exitButton.setColor(Color(exitButton.getColor().r, exitButton.getColor().g, exitButton.getColor().b, 255));

	drawButtons(window);
}

void MenuMaker::drawButtons(RenderWindow &window)
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
	
