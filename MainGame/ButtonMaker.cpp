#include "ButtonMaker.h"

ButtonMaker::ButtonMaker()
{
}

void ButtonMaker::initialize(Texture &textureD, Texture &textureS, Texture &textureP, Vector2f position, Vector2f size, bool isSelectable, ButtonTag tag)
{
	this->tag = tag;
	this->textureDefault = textureD;
	this->texturePressed = textureP;
	this->textureSelected = textureS;
	this->isSelectable = isSelectable;
	this->spriteDefault.setTexture(this->textureDefault);
	this->spritePressed.setTexture(this->texturePressed);
	this->spriteSelected.setTexture(this->textureSelected);
	this->spriteDefault.setPosition(position);
	this->spritePressed.setPosition(position);
	this->spriteSelected.setPosition(position);
	Vector2f textureSize = Vector2f (this->textureDefault.getSize());
	this->spriteDefault.setScale(size.x / textureSize.x, size.y / textureSize.y);
	this->spritePressed.setScale(size.x / textureSize.x, size.y / textureSize.y);
	this->spriteSelected.setScale(size.x / textureSize.x, size.y / textureSize.y);
}

bool ButtonMaker::isSelected(Vector2f mousePos)
{
	if (!isActive)
		return false;

	//isActive = false;

	FloatRect rect = spriteDefault.getGlobalBounds();
	if (rect.contains(Vector2f(mousePos)))
	{
		selected = true;
		return true;		
	}
	else
	{
		selected = false;
		return false;
	}

	selected = false;
	return false;
}

void ButtonMaker::setPosition(Vector2f position) 
{
	spriteDefault.setPosition(position);
	spritePressed.setPosition(position);
	spriteSelected.setPosition(position);
}

void ButtonMaker::setSize(Vector2f size)
{
	this->spriteDefault.setScale(size.x / textureDefault.getSize().x, size.y / textureDefault.getSize().y);
	this->spritePressed.setScale(size.x / texturePressed.getSize().x, size.y / texturePressed.getSize().y);
	this->spriteSelected.setScale(size.x / textureSelected.getSize().x, size.y / textureSelected.getSize().y);
}

void ButtonMaker::draw(RenderWindow &window)
{
	isActive = true;

	Color spriteColor = this->spriteDefault.getColor();
	if (isSelected(Vector2f(Mouse::getPosition())) && isSelectable)
	{
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
			window.draw(this->spritePressed);
		else
			window.draw(this->spriteSelected);
	}
	else
		window.draw(this->spriteDefault);
}
