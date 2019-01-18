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
	font.loadFromFile("fonts/normal.ttf");
	Text result(text, font, size);
	result.setFillColor(Color::White);
	Vector2f pos(static_cast<float>(Posx),static_cast<float>(Posy));
	result.setPosition(Vector2f(pos));
	window->draw(result);
}

void Helper::drawTextWithSettings(std::string text, int size, int Posx, int Posy, Color color, RenderWindow *window)
{
	Font font;
	font.loadFromFile("fonts/normal.ttf");
	Text result(text, font, size);
	result.setFillColor(color);
	Vector2f pos(static_cast<float>(Posx), static_cast<float>(Posy));
	result.setPosition(Vector2f(pos));
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

Side Helper::getSide(Vector2f position, Vector2f anotherPosition)
{
	Side answer;
	float alpha = atan((float(anotherPosition.y) - position.y) / (float(anotherPosition.x) - position.x)) * 180 / pi;
	if (position.y >= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = up;
	else
		if (position.x <= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = right;
		else
			if (position.y <= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = down;
			else
				if (position.x >= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = left;
	return answer;
}

RectangleShape Helper::makeLine(Vector2f point1, Vector2f point2)
{
	if (point1.y >= point2.y)
		std::swap(point1, point2);

	point1.y -= 4;

	RectangleShape result;
	result.setPosition(point1);
	result.setSize(Vector2f(sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)), 8));

	if (point1.x >= point2.x)
		result.rotate(acos(abs(point1.y - point2.y) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180 + 90);
	else
		result.rotate(acos(abs(point1.x - point2.x) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180);
	result.setFillColor(Color::Black);
	return result;
}

