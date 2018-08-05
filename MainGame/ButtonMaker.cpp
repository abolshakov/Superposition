#include "ButtonMaker.h"

ButtonMaker::ButtonMaker()
{
}

void ButtonMaker::initialize(Texture &texture, Vector2f position, Vector2f size, ButtonTag tag)
{
	this->tag = tag;

	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(position);
	Vector2f textureSize = Vector2f(this->texture.getSize());
	this->sprite.setScale(size.x / textureSize.x, size.y / textureSize.y);
}

void ButtonMaker::initialize(Texture &texture, FloatRect rect, ButtonTag tag)
{
	this->tag = tag;

	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(rect.left, rect.top);
	Vector2f textureSize = Vector2f(this->texture.getSize());
	this->sprite.setScale(rect.width / textureSize.x, rect.height / textureSize.y);
}

bool ButtonMaker::isSelected(Vector2f mousePos)
{
	FloatRect rect = sprite.getGlobalBounds();
	if (rect.contains(mousePos))
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

void ButtonMaker::draw(RenderWindow &window)
{
	Color spriteColor = this->sprite.getColor();
	if (isSelected(Vector2f(Mouse::getPosition())))
		this->sprite.setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 125));
	else
		this->sprite.setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 255));

	window.draw(this->sprite);
}
