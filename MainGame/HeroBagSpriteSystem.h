#ifndef HBSPRITESYSTEM_H
#define HBSPRITESYSTEM_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Helper.h"

enum class SpriteId { bagClosed = 1, bagClosedHover = 2, bagOpen = 3, bagOpenHover = 4 };

using namespace  sf;

class HeroBagSpriteSystem
{
private:
	std::unordered_map<SpriteId, Sprite> spriteMap;
	std::unordered_map<SpriteId, Texture> textures;
	Vector2f openBagSize, closedBagSize;
public:
	HeroBagSpriteSystem();
	~HeroBagSpriteSystem();
	void initSpriteMap();
	std::string idToString(SpriteId type);
};

#endif

