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
	result.setFillColor(Color::White);
	Vector2f pos(static_cast<float>(Posx),static_cast<float>(Posy));
	result.setPosition(pos);
	window->draw(result);
}

void Helper::drawTextWithSettings(std::string text, int size, int Posx, int Posy, Color color, RenderWindow *window)
{
	Font font;
	font.loadFromFile("fonts/normal.ttf");
	Text result(text, font, size);
	result.setFillColor(color);
	Vector2f pos(static_cast<float>(Posx), static_cast<float>(Posy));
	result.setPosition(pos);
	window->draw(result);
}

std::string Helper::withoutNums(std::string s)
{
	bool isFinish;
	while (true)
	{
		isFinish = true;
		for (int i = 0; i < s.length(); i++)
		{
			if (s[i] >= '0' && s[i] <= '9')
			{
				s.erase(i, 1);
				isFinish = false;
			}
		}
		if (isFinish)
			break;
	}
	return s;
}

std::string Helper::getObjectName(std::string s)
{
	std::string answer = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != '.')
			answer += s[i];
		else
			break;
	}
	return answer;
}

std::string Helper::getSpriteName(std::string s)
{
	for (int i = s.length() - 1; i >= 0; i--)
	{
		if (s[i] >= '0' && s[i] <= '9')
			s.erase(i, 1);
		else
			return s;
	}
}

bool Helper::isIntersects(Vector2f pos, IntRect shape)
{
	if (pos.x >= shape.left && pos.x <= shape.left + shape.width && pos.y >= shape.top && pos.y <= shape.top + shape.height)
		return true;
	return false;
}

bool Helper::isIntersectTerrain(Vector2f position, TerrainObject& terrain, float radius)
{
	auto f1 = terrain.getFocus1();
	auto f2 = terrain.getFocus2();
	return (sqrt((position.x - f1.x)*(position.x - f1.x) + (position.y - f1.y)*(position.y - f1.y)) + sqrt((position.x - f2.x)*(position.x - f2.x) + (position.y - f2.y)*(position.y - f2.y))/* - dynamic.radius*/) <= terrain.getEllipseSize() + radius;
}

