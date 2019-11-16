#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "HeroBookPage.h"
#include "ButtonMaker.h"
#include "HeroBag.h"
#include "TextWriter.h"
#include "WorldObject.h"

using namespace sf;

class HeroBook
{
public:
	HeroBook();
	~HeroBook();
	void draw(sf::RenderWindow* window, float hpRatio, float elapsedTime);
	void interact(float elapsedTime);
	void onMouseUp();
	void getAllOuterInfo(
		std::vector<HeroBag>* bags,
		std::string name,
		WorldObject* object,
		Cell* worldHeldItem,
		bool nearTheTable = false);
	void WhileMouseDown();
	void setPage(int page);
	void changeVisibility() { this->visibility = !this->visibility; }
	int getCurrentPage() const { return currentPage; }
	
	void boundHeldItem() { this->worldHeldItem = worldHeldItem; }
	std::string debugInfo = "";
	Vector2f getHpLinePosition() { return {	buttonList.at(ButtonTag::bookStandTag).getPosition().x + buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().width - 40,
		buttonList.at(ButtonTag::bookStandTag).getPosition().y + buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().height / 2 - 20 }; }
private:
	HeroBookPage somePage;
	void drawHpLine(RenderWindow* window, float hpRatio);
	void drawWreathMatrix(RenderWindow* window, pageContent content);
	void drawPlantsMatrix(RenderWindow* window);
	void drawLineMatrix(RenderWindow* window);
	void drawPlantsList(RenderWindow* window);
	void drawWreathCost(Vector2f pos, RenderWindow* window);
	void initButtons();
	void initContent();

	std::string buttonsInfoFileDirectory = "Game/heroBook/buttonsInfo.txt",
		pagesInfoFileDirectory = "Game/heroBook/pagesInfo.txt",
		contentInfoFileDirectory = "Game/heroBook/bookContent.txt";
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	int currentPage = 0;
	/////////////////////////
	// creatures 101 - 199 //
	// craft 201 - 299     //
	// plants 301 - 399    //
	// wreath 401 - 499    //
	/////////////////////////
	TextWriter textWriter;
	bool visibility = false;
	std::pair<Tag, int> heldItem = { Tag::emptyCell, 0 };
	Cell* worldHeldItem = nullptr;
	WorldObject* worldSelectedObject = nullptr;
	Tag currentDraft = Tag::emptyDraft, currentFlower = Tag::emptyCell;
	std::string worldMouseName = "";
	Vector2f blockDescriptionPoint = Vector2f(0.53f, 0.104f); // as a percentage of the size of the page markup
	Vector2f pageDescriptionPoint = Vector2f(0.53f, 0.395f); // as a percentage of the size of the page markup
	std::vector<HeroBag>* boundBags;
};

