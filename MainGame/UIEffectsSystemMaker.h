#pragma once
#ifndef UIEFFECTSSYSTEMMAKER_H
#define UIEFFECTSSYSTEMMAKER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include "Helper.h"
#include "BoardSprite.h"

using namespace  sf;

enum class UIEffects { transparencyRemoval = 1 };

struct uiEffect
{
	uiEffect() : body(nullptr), duration(0) {}
	uiEffect(Drawable* body, float duration) : body(body), duration(duration) {}
	Drawable* body;
	float duration;
	float timePassed = 0;
	//std::string name;
};

class UIEffectsSystemMaker
{
private:
	std::unordered_map<std::string, uiEffect> transparencyRemoval;
	std::unordered_set<std::string> names;

public:
	UIEffectsSystemMaker();
	~UIEffectsSystemMaker();
	void init();
	void resetEffects(std::vector<std::string> removeList);
	void addEffect(UIEffects effect, Drawable* elem, std::string name, float duration = defaultDuration);
	void interact(float elapsedTime);

	static float defaultDuration;
};

#endif