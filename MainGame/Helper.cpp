#include "Helper.h"

Vector2f Helper::GetScreenSize()
{
	Vector2f screenSize;
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		screenSize.x = lpDevMode.dmPelsWidth;
		screenSize.y = lpDevMode.dmPelsHeight;
	}
	return screenSize;
}

int Helper::getFps()
{
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		return lpDevMode.dmDisplayFrequency;
	}
}

void Helper::drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window)
{
	Font font;
	font.loadFromFile("fonts/Bebas.ttf");
	Text result(text, font, size);
	result.setFillColor(Color::Red);
	Vector2f pos;
	pos.x = Posx;
	pos.y = Posy;
	result.setPosition(pos);
	window->draw(result);
}


