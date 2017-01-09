#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Helper
{
public:
	static Vector2f GetScreenSize();
	static int getFps();
	static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
};


