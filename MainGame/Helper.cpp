#include <windows.h>
#include "Helper.h"

Vector2f Helper::GetScreenSize()
{
	Vector2f screenSize;
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		screenSize.x = float(lpDevMode.dmPelsWidth);
		screenSize.y = float(lpDevMode.dmPelsHeight);
	}
	return screenSize;
}

int Helper::getFps()
{
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		return lpDevMode.dmDisplayFrequency;
	}
	return 0;
}

void Helper::drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window)
{
	Font font;
	font.loadFromFile("fonts/Bebas.ttf");
	Text result(text, font, size);
	result.setFillColor(Color::Red);
	Vector2f pos(static_cast<float>(Posx),static_cast<float>(Posy));
	result.setPosition(pos);
	window->draw(result);
}


