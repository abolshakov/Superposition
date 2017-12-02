#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Helper.h"
#include <array>

using namespace sf;
using namespace std;

struct objectInfo {
	string type, image, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	vector<pair<int, int>> recipe;
};

class Builder
{
protected:

public:	
	Builder();
	void draw(RenderWindow &window, World& world, float elapsedTime);
	void onMouseDownInteract(World& world);
	void interact();
	void setCharacterBuildActivity(Deerchant& hero);
private:	
	void inicializeObjectsInfo();
	void animator(float elapsedTime);
	void inicialize(Vector2f position, World& world);
	float originalAnimationSpeed = 0.001, animationSpeed = 0.001;
	bool isBuilding = false, isPressed = false, buildingAvaliable = false, isRecipeFrame = false;
	Sprite buildStartButton, buildStopButton, recipeFrame;
	objectInfo builtObjects[10];
	Texture buildStartButtonTexture, buildStopButtonTexture, recipeFrameTexture;
	int currentObject = 0;
	//array<string, 10> builtObjectsIcons{ { "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png", "World/Tree.png" } },
	//builtObjectsTypes{ { "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", "roseTree", } },
	//builtObjectsNamesOfImages{ { "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png", "roseTree3.png" } };
};

