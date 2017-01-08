#include <windows.h>
#include <SFML/Graphics.hpp>

using namespace sf;
static class Helper
{
public:
	static Vector2f GetScreenSize();
	static int getFps();
	static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
};


