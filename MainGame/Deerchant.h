#pragma once
#include "DynamicObject.h"

using namespace sf;

struct bodyElements
{
	Texture texture;
	Sprite sprite;
	std::string name;
};

class Deerchant : public DynamicObject
{
private:
	float lastTime, timeForNewImage;
	Clock clock;
	int bodyElementsNumber;
	int currentImg;
	int imgInRunAnimation;

public:
	std::string nameOfFile;
	Deerchant(Vector2f position, Vector2u size, std::string name, std::string type);
	void move(float time);
};

