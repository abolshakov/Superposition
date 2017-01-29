#pragma once
#include "DynamicObject.h"

using namespace sf;

class Deerchant : public DynamicObject
{
	float timeForNewSprite;
	int currentSprite;
	int animationLength;
public:
	std::string nameOfFile;
	Deerchant(std::string objectName, Vector2f centerPosition);
	Vector2i calculateTextureOffset() override;
	std::string getSpriteName(long long elapsedTime) override;
	void handleInput() override;
};

