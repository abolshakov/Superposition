#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace sf;

enum FontName { BebasFont = 1, NormalFont = 2 };

class TextWriter
{
private:
	void initFonts();
	void initTextBoxes();

	std::unordered_map<FontName, Font> fonts;
	std::unordered_map<FontName, Text> textBoxes;
public:
	TextWriter();
	~TextWriter();
	void drawString(std::string str, FontName font, int size, float posX, float posY, RenderWindow* window, Color color = Color::Black);
	void drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderWindow* window, Color color = Color::Black);
};

