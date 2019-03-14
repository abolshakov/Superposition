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
	HeroBookPage somePage;
	void drawHpLine(RenderWindow* window, float hpRatio);
	void initButtons();
	void initContent();

	std::string buttonsInfoFileDirectory = "Game/heroBook/buttonsInfo.txt",
		pagesInfoFileDirectory = "Game/heroBook/pagesInfo.txt",
		contentInfoFileDirectory = "Game/heroBook/bookContent.txt";
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	std::vector<std::vector<std::vector<contentBlock>>> allContentChains;
	int currentPage = 0;
	//------------------
	//1000 pages allocated to sections:
	//animals: 1 - 100
	//plants: 101 - 200
	//items: 201 - 300
	//wreaths: 301 - 400
	//nightmare: 401 - 500
	//
	//lists: 901 - 905
	//901 - animals
	//902 - plants
	//903 - items
	//904 - wreaths
	//905 - nightmare
	//-------------------
	TextWriter textWriter;
	bool visibility = false;
public:
	HeroBook();
	~HeroBook();
	void draw(sf::RenderWindow* window, float hpRatio, float elapsedTime);
	void onMouseDown();
	void setPage(int page);
	void changeVisibility() { this->visibility = !this->visibility; }
	int getCurrentPage() const { return currentPage; }

	Vector2f getHpLinePosition() { return Vector2f(buttonList.at(bookStandTag).getPosition().x + buttonList.at(bookStandTag).getGlobalBounds().width - 40,
		buttonList.at(bookStandTag).getPosition().y + buttonList.at(bookStandTag).getGlobalBounds().height / 2 - 20); }
};

