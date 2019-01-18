#include "TextWriter.h"



TextWriter::TextWriter()
{
	initFonts();
	initTextBoxes();
}


TextWriter::~TextWriter()
{
}

void TextWriter::initFonts()
{
	Font currentFont;
	currentFont.loadFromFile("fonts/Bebas.ttf");
	fonts.insert({BebasFont, currentFont});
	currentFont.loadFromFile("fonts/normal.ttf");
	fonts.insert({ NormalFont, currentFont });
}

void TextWriter::initTextBoxes()
{
	Text currentText;
	currentText.setFont(fonts[BebasFont]);
	textBoxes.insert({ BebasFont, currentText });
	currentText.setFont(fonts[NormalFont]);
	textBoxes.insert({ NormalFont, currentText });
}

void TextWriter::drawString(std::string str, FontName font, int size, float posX, float posY, RenderWindow* window)
{
	textBoxes.at(font).setPosition(posX, posY);
	textBoxes.at(font).setCharacterSize(size);
	textBoxes.at(font).setFillColor(Color::Black);
	textBoxes.at(font).setString(str);
	window->draw(textBoxes.at(font));
}

void TextWriter::drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderWindow* window)
{
	auto curText = textBoxes.at(font);
	curText.setString('a');
	curText.setCharacterSize(size);

	float curPosY = posY + float(size) / 2, curPosX = posX + float(size) / 2;

	while (true)
	{
		if (curPosY > posY + height)
			break;
		if (str.length() + 1 < width / size)
		{
			drawString(str, font, size, curPosX, curPosY, window);
			break;
		}
		else
		{
			drawString(str.substr(0, (width / curText.getGlobalBounds().width) - 1), font, size, curPosX, curPosY, window);
			curPosY += size;
			str.erase(0, width / size - 1);
		}
	}
}


