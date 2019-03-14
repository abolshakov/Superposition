#pragma once
#ifndef MENUMAKER_H
#define MENUMAKER_H

#include <SFML/Graphics.hpp>
#include "World.h"
#include "Helper.h"
#include "ButtonMaker.h"

using namespace sf;

enum MenuStates { mainMenu = 1, gameMenu = 2, closed = 3 };

class MenuMaker
{
protected:

public:
	MenuMaker();
	~MenuMaker();	
	void interact(World &world, RenderWindow &window);
	void drawButtons(RenderWindow &window);	
	MenuStates getState() { return menuState; }
	void setState(MenuStates state) { menuState = state; }
	void onKeyDown(Event event, World &world);
	bool getActivity() { return wasActive; }
private:	
	MenuStates menuState = mainMenu;
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	std::string buttonsInfoFileDirectory = "Game/MenuMaker/buttonsInfo.txt";
	void initButtons();
	bool wasActive;
	Vector2f screenSize;
	/*typedef World datatype;
	datatype blankref;
	std::reference_wrapper<datatype> world = blankref;*/
};
#endif