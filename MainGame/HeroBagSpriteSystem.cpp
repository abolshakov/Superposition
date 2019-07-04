#include "HeroBagSpriteSystem.h"

HeroBagSpriteSystem::HeroBagSpriteSystem()
{
	initSpriteMap();
}

HeroBagSpriteSystem::~HeroBagSpriteSystem()
{
}

void HeroBagSpriteSystem::initSpriteMap()
{
	openBagSize = Vector2f(Helper::GetScreenSize().x / 6, Helper::GetScreenSize().y / 3);
	closedBagSize = Vector2f(Helper::GetScreenSize().x / 12, Helper::GetScreenSize().y / 6);
	for (int cnt = 1; cnt <= 4; cnt++)
	{
		textures[SpriteId(cnt)] = *(new Texture());
		spriteMap[SpriteId(cnt)] = *(new Sprite());
		textures.at(SpriteId(cnt)).loadFromFile(idToString(SpriteId(cnt)));
		spriteMap.at(SpriteId(cnt)).setTexture(textures.at(SpriteId(cnt)));
		if (cnt >= 1 && cnt <= 2)
			spriteMap.at(SpriteId(cnt)).setScale(closedBagSize.x / textures.at(SpriteId(cnt)).getSize().x, closedBagSize.y / textures.at(SpriteId(cnt)).getSize().y);
		if (cnt >= 3 && cnt <= 4)
			spriteMap.at(SpriteId(cnt)).setScale(openBagSize.x / textures.at(SpriteId(cnt)).getSize().x, openBagSize.y / textures.at(SpriteId(cnt)).getSize().y);
	}

}

std::string HeroBagSpriteSystem::idToString(SpriteId type)
{
	if (type == SpriteId::bagClosed)
		return "Game/inventorySprites/bag1Icon.png";
	if (type == SpriteId::bagClosedHover)
		return "Game/inventorySprites/bag1IconHover.png";
	if (type == SpriteId::bagClosed)
		return "Game/inventorySprites/bag1.png";
	if (type == SpriteId::bagClosedHover)
		return "Game/inventorySprites/bag1Hover.png";
}

