#pragma once
#include <SFML/Graphics.hpp>
#include "TerrainObject.h"
#include "DynamicObject.h"

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
	static Side getSide(Vector2f position, Vector2f anotherPosition);
	static float getDist(Vector2f a, Vector2f b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }
	static float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3) { return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1); }
};


