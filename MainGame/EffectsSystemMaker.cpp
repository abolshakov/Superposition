#include "EffectsSystemMaker.h"



EffectsSystemMaker::EffectsSystemMaker()
{
}


EffectsSystemMaker::~EffectsSystemMaker()
{
}

void EffectsSystemMaker::init(std::unordered_map<std::string, BoardSprite>* spriteMap)
{
	this->spriteMap = spriteMap;
	this->keyTimePoints[Effects::fog] = 200000;
	this->spriteCounter[Effects::fog] = 1;
	this->animationSpeed[Effects::fog] = 3500;
	this->maxSpriteNumber[Effects::fog] = 150;
	this->spriteNames[Effects::fog] = "Game/shaders/fog/fog";
}

void EffectsSystemMaker::drawFog(RenderWindow* window, float elapsedTime)
{
	if (this->timers[Effects::fog] >= 10e6 / this->animationSpeed[Effects::fog])
	{
		if (++this->spriteCounter[Effects::fog] > this->maxSpriteNumber[Effects::fog])
			this->spriteCounter[Effects::fog] = 1;
		this->timers[Effects::fog] = 0;
	}
	else
		this->timers[Effects::fog] += elapsedTime;

	auto& sprite = spriteMap->at(spriteNames[Effects::fog] + std::to_string(this->spriteCounter[Effects::fog]) + ".png").sprite;
	auto& texture = spriteMap->at(spriteNames[Effects::fog] + std::to_string(this->spriteCounter[Effects::fog]) + ".png").texture;
	sprite.setScale(Helper::GetScreenSize().x / texture.getSize().x, Helper::GetScreenSize().y / texture.getSize().y);
	sprite.setPosition(0, 0);
	sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 200));
	window->draw(sprite);
}

void EffectsSystemMaker::draw(RenderWindow* window, float elapsedTime)
{
	drawFog(window, elapsedTime);
}
