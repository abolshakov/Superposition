#include "MenuMaker.h"

MenuMaker::MenuMaker()
{
	initButtons();
}

/*void MenuMaker::worldBounding(World &world)
{
	this->world = world;
}*/

void MenuMaker::initButtons()
{
	screenSize = Helper::GetScreenSize();

	std::string buttonImagePath;
	Vector2f buttonPosition, buttonScale; // in percents
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);
	while (fin >> buttonImagePath >> buttonPosition.x >> buttonPosition.y >> buttonScale.y >> isSelectable >> tag)
	{
		Texture buttonTexture;
		buttonTexture.loadFromFile(buttonImagePath);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonScale.y = buttonScale.y * screenSize.y / 100;
		buttonScale.x = buttonScale.y * (buttonTexture.getSize().x / buttonTexture.getSize().y);
		
		buttonList[ButtonTag(tag)].initialize(buttonTexture, buttonPosition, buttonScale, isSelectable, ButtonTag(tag));
	}

	fin.close();
}

MenuMaker::~MenuMaker()
{

}

void MenuMaker::onKeyDown(Event event, World &world)
{
	if (event.key.code == Keyboard::Escape)
	{
		if (menuState == closed)
		{
			menuState = gameMenu;	
		}
		else
			if (menuState == gameMenu)
			{
				menuState = closed;
			}
	}
}

void MenuMaker::interact(World &world, RenderWindow &window)
{
	if (menuState == closed)
		return;

	wasActive = false;

	Vector2f mousePos = (Vector2f )Mouse::getPosition();

	if (menuState == mainMenu)
	{
		if (buttonList[newRunTag].isSelected(mousePos))
		{
			world.generate(500);
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[continueTag].isSelected(mousePos))
		{
			world.Load();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[exitTag].isSelected(mousePos))
		{
			menuState = closed;
			window.close();
			wasActive = true;
			return;
		}

		return;
	}

	if (menuState == gameMenu)
	{
		if (buttonList[newRunTag].isSelected(mousePos))
		{
			world.generate(500);
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[continueTag].isSelected(mousePos))
		{
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[menuTag].isSelected(mousePos))
		{
			world.Save();
			menuState = mainMenu;
			wasActive = true;
			return;
		}

		return;
	}
}

void MenuMaker::drawButtons(RenderWindow &window)
{
	if (menuState == mainMenu)
	{
		buttonList[newRunTag].draw(window);
		buttonList[continueTag].draw(window);
		buttonList[settingsTag].draw(window);
		buttonList[exitTag].draw(window);

		return;
	}

	if (menuState == gameMenu)
	{
		buttonList[newRunTag].draw(window);
		buttonList[continueTag].draw(window);
		buttonList[settingsTag].draw(window);
		buttonList[menuTag].draw(window);

		return;
	}
}
	
