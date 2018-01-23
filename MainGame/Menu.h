#pragma once
#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "World.h"
#include "Helper.h"

using namespace sf;

class Menu
{
protected:

public:
	bool isMenu = true;
	Menu();
	void drawLoadingImage(RenderWindow &window);
	void interact(World &world, RenderWindow &window);
	void drawButtons(RenderWindow &window);
private:
	Sprite newRunButton, continueButton, exitButton, settingsButton;
	Texture newRunButtonTexture, continueButtonTexture, exitButtonTexture, settingsButtonTexture;
	Vector2f newRunButtonSize, continueButtonSize, exitButtonSize, settingsButtonSize;
	Vector2f newRunButtonPosition, continueButtonPosition, exitButtonPosition, settingsButtonPosition;
	bool newRunButtonShow = true, continueButtonShow = true, exitButtonShow = true, settingsButtonShow = true;

	Sprite loadingImage;
	Texture loadingImageTexture;

	Vector2f screenSize;
};
#endif