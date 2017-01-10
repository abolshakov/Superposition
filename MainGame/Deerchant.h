#pragma once
#include "DynamicObject.h"

using namespace sf;

class Deerchant : public DynamicObject
{
private:
	float timeForNewSprite;
	int currentSprite;
	int animationLength;
public:
	std::string nameOfFile;
	Deerchant(Vector2f position, Vector2u size, std::string name);
	std::string getSpriteName(long long elapsedTime) override;
	void handleInput() override;
};

