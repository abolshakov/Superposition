#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "HeroBookPage.h"
#include "ButtonMaker.h"
#include "World.h"
#include "TextWriter.h"

using namespace sf;

class HeroBook
{
private:
	World* boundedWorld;
	HeroBookPage somePage;
	void initButtons();
	void initPages();
	void initContent();

	std::string buttonsInfoFileDirectory = "Game/heroBook/buttonsInfo.txt",
		pagesInfoFileDirectory = "Game/heroBook/pagesInfo.txt",
		contentInfoFileDirectory = "Game/heroBook/bookContent.txt";
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	std::vector<std::vector<std::vector<contentBlock>>> allContentChains;
	int currentPage = 2;
	TextWriter textWriter;
public:
	HeroBook();
	~HeroBook();
	void worldBinding(World* world) { this->boundedWorld = world; }
	void draw(sf::RenderWindow* window, float elapsedTime);
	void onMouseDown();
	void setPage(int page);
};

