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
	Vector2f screenSize = Helper::GetScreenSize();

	std::string buttonImagePathDefault, buttonImagePathPressed, buttonImagePathSelected;
	Vector2f buttonPosition, buttonSize; // in percents
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);


	while (fin >> isSelectable)
	{
		if (isSelectable)
			fin >> buttonImagePathDefault >> buttonImagePathPressed >> buttonImagePathSelected;
		else
			fin >> buttonImagePathDefault;

		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> tag;

		Texture buttonTextureDefault, buttonTexturePressed, buttonTextureSelected;
		buttonTextureDefault.loadFromFile(buttonImagePathDefault);
		buttonTexturePressed.loadFromFile(buttonImagePathPressed);
		buttonTextureSelected.loadFromFile(buttonImagePathSelected);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = buttonTextureDefault.getSize().x * buttonSize.y / buttonTextureDefault.getSize().y;

		buttonList[ButtonTag(tag)].initialize(buttonTextureDefault, buttonTexturePressed, buttonTextureSelected, buttonPosition, buttonSize, isSelectable, ButtonTag(tag));
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

	/*if (menuState == closed)
	{
		if (buttonList.at(openMenu).isSelected(mousePos))
		{
			menuState = gameMenu;
			wasActive = true;
			return;
		}
	}*/
}

void MenuMaker::drawButtons(RenderWindow &window)
{
	if (menuState == mainMenu)
	{
		buttonList.at(newRunTag).draw(window);
		buttonList.at(continueTag).draw(window);
		buttonList.at(settingsTag).draw(window);
		buttonList.at(exitTag).draw(window);
		return;
	}

	if (menuState == gameMenu)
	{
		buttonList.at(newRunTag).draw(window);
		buttonList.at(continueTag).draw(window);
		buttonList.at(settingsTag).draw(window);
		buttonList.at(menuTag).draw(window);
		return;
	}

	if (menuState == closed)
	{
		//buttonList.at(openMenu).draw(window);
		return;
	}
}
	
