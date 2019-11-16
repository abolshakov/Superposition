#pragma once
#ifndef SPRITETEXTURES_H
#define SPRITETEXTURES_H

#include <SFML/Graphics.hpp>

using namespace sf;

struct BoardSprite
{
	Texture texture;
	Sprite sprite;
};

struct CellSprite {
	Sprite sprite;
	Texture texture;
	Vector2f offset;
};

struct BagSprite
{
	Sprite CBag, CBagS, OBag, OBagS;
	Texture TextureCBag, TextureCBagS, TextureOBag, TextureOBagS;
};

#endif
