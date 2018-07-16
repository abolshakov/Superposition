#include "ButtonMaker.h"

ButtonMaker::ButtonMaker()
{
}

void ButtonMaker::initialize(Texture &texture, Vector2f position, Vector2f size, Tags tag)
{
	this->tag = tag;

	this->texture = texture;
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(position);
	Vector2f textureSize = Vector2f(this->texture.getSize());
	this->sprite.setScale(size.x / textureSize.x, size.y / textureSize.y);
}

void ButtonMaker::initialize(Texture &texture, FloatRect rect, Tags tag)
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
		return true;
	else
		return false;

	return false;
}

void ButtonMaker::draw(RenderWindow &window)
{
	window.draw(this->sprite);
}
