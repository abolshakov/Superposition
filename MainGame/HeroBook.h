#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "HeroBookPage.h"
#include "ButtonMaker.h"
#include "HeroBag.h"
#include "TextWriter.h"

using namespace sf;

class HeroBook
{
private:
	HeroBookPage somePage;
	void drawHpLine(RenderWindow* window, float hpRatio);
	void drawWreathMatrix(RenderWindow* window, pageContent content);
	void drawPlantsMatrix(RenderWindow* window);
	void initButtons();
	void initContent();

	std::string buttonsInfoFileDirectory = "Game/heroBook/buttonsInfo.txt",
		pagesInfoFileDirectory = "Game/heroBook/pagesInfo.txt",
		contentInfoFileDirectory = "Game/heroBook/bookContent.txt";
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	std::vector<std::vector<std::vector<contentBlock>>> allContentChains;
	int currentPage = 0;
	TextWriter textWriter;
	bool visibility = false;
	std::pair<lootItemsIdList, int> heldItem = {lootItemsIdList::bagCell, 0};
	bagCell* worldHeldItem = nullptr;
	ButtonTag currentDraft = ButtonTag::emptyDraftCenter;
public:
	HeroBook();
	~HeroBook();
	void draw(sf::RenderWindow* window, float hpRatio, float elapsedTime);
	void interact(float elapsedTime);
	void onMouseUp();
	void WhileMouseDown();
	void setPage(int page);
	void changeVisibility() { this->visibility = !this->visibility; }
	int getCurrentPage() const { return currentPage; }
	void boundHeldItem(bagCell* worldHeldItem) { this->worldHeldItem = worldHeldItem; }
	std::string debugInfo = "";
	Vector2f getHpLinePosition() { return Vector2f(buttonList.at(ButtonTag::bookStandTag).getPosition().x + buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().width - 40,
		buttonList.at(ButtonTag::bookStandTag).getPosition().y + buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().height / 2 - 20); }
};

