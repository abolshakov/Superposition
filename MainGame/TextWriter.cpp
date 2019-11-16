#include "TextWriter.h"
#include "HeroBag.h"

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

	numberOfItems.setFont(fonts[BebasFont]);
	numberOfItems.setCharacterSize(30);
	numberOfItems.setFillColor(Color::White);
}

void TextWriter::drawString(std::string str, FontName font, int size, float posX, float posY, RenderWindow* window, Color color)
{
	textBoxes.at(font).setPosition(posX, posY);
	textBoxes.at(font).setCharacterSize(size);
	textBoxes.at(font).setFillColor(color);
	textBoxes.at(font).setString(str);
	window->draw(textBoxes.at(font));
}

void TextWriter::drawTextBox(std::string str, FontName font, int size, float posX, float posY, float width, float height, RenderWindow* window, Color color)
{
	auto curText = textBoxes.at(font);
	curText.setString(str);
	const int lineLength = str.size() * (width / curText.getGlobalBounds().width);
	float curPosY = posY;

	if (curText.getGlobalBounds().width <= width)
	{
		drawString(str, font, size, posX, curPosY, window, color);
		return;
	}

	while (!str.empty())
	{
		if (curPosY > posY + height - curText.getGlobalBounds().height / 2)
			return;
		
		int spacePos = std::min(lineLength, int(str.size() - 1));
		if (str.length() > lineLength)
		{
			while (!(str[spacePos] == ' ' || str[spacePos] == '_') && spacePos > 0)
				spacePos--;
		}
		else
			spacePos = 0;

		if (spacePos != 0)
			str.erase(str.begin() + spacePos);
		else
			spacePos = lineLength;

		drawString(str.substr(0, spacePos), font, size, posX, curPosY, window, color);
		curPosY += curText.getGlobalBounds().height;
		str.erase(0, spacePos);
	}
}

void TextWriter::drawNumberOfItems(Vector2f pos, int itemsCount, RenderWindow &window)
{
	numberOfItems.setString(std::to_string(itemsCount));
	numberOfItems.setOrigin(numberOfItems.getGlobalBounds().width, numberOfItems.getGlobalBounds().height);
	numberOfItems.setPosition(pos.x + HeroBag::itemCommonRadius * 2, pos.y + HeroBag::itemCommonRadius * 2);
	window.draw(numberOfItems);
}


