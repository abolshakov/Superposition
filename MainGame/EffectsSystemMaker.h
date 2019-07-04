#pragma once
#ifndef EFFECTSSYSTEMMAKER_H
#define EFFECTSSYSTEMMAKER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Helper.h"
#include "BoardSprite.h"

using namespace  sf;

enum class Effects { fog = 1 };

class EffectsSystemMaker
{
private:
	std::unordered_map<std::string, BoardSprite>* spriteMap = nullptr;
	std::unordered_map<Effects, float> timers, keyTimePoints, animationSpeed; //speed = frames per sec
	std::unordered_map<Effects, int> spriteCounter, maxSpriteNumber;
	std::unordered_map<Effects, std::string> spriteNames;

public:
	EffectsSystemMaker();
	~EffectsSystemMaker();
	void init(std::unordered_map<std::string, BoardSprite>* spriteMap);
	void draw(RenderWindow* window, float elapsedTime);
	void drawFog(RenderWindow* window, float elapsedTime);
};

#endif