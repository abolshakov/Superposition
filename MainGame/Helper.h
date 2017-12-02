#pragma once
#include <SFML/Graphics.hpp>
#include "TerrainObject.h"

using namespace sf;
class Helper
{
public:
	static Vector2f GetScreenSize();
	static int getFps();
	static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
	static void drawTextWithSettings(std::string text, int size, int Posx, int Posy, Color color, RenderWindow *window);
	static std::string withoutNums(std::string s);
	static std::string getObjectName(std::string s);
	static std::string getSpriteName(std::string s);
	static bool isIntersects(Vector2f pos, IntRect shape);
	static bool isIntersectTerrain(Vector2f position, TerrainObject& terrain, float radius);
};


