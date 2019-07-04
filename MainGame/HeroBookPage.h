#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <fstream>

#include "WreathDraft.h"

struct contentBlock
{
	ButtonTag tag;
	std::vector<std::pair<int, int>> connections;
	std::string textDescription;
};

struct pageContent
{
	std::vector<ButtonTag> buttons;
	std::string blockDescription;
	std::vector<RectangleShape> connections;
};

class HeroBookPage
{
private:
	static int div_up(int n, int d) { return n / d + (((n < 0) ^ (d > 0)) && (n % d)); }
	std::vector<std::vector<std::vector<contentBlock>>> allContentChains;
	std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList;
	void setBookmarkPosition();
	void preparePageBase();
	void putBlocksToPositions(std::vector<ButtonTag> buttons);
	void initDrafts();

	int currentPage = 0;
	FloatRect pageGlobalBounds;
	FloatRect blockGlobalBounds, arrowToPageGlobalBounds;
	Vector2f leftPagePosition, rightPagePosition;
	std::unordered_map<ButtonTag, WreathDraft> originalSetups;

public:
	HeroBookPage();
	~HeroBookPage();
	void buttonListBounding(std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList);
	void setContentChain(std::vector<std::vector<std::vector<contentBlock>>> allContentChains);
	void setPage(int page) { this->currentPage = page; }
	void onMouseDown();
	std::pair<int, int> getSelectedWreathCell();
	std::pair<int, int> getSelectedPlantsCell();
	std::vector<std::pair<int, int>> getBorderCells(int raw, int column);

	static bool checkWreathCellFit(int i, int j, std::vector<int> rings = std::vector<int>());
	std::string debugInfo = "";
	pageContent getPreparedContent(int pageNumber, ButtonTag currentDraft = ButtonTag::emptyDraftCenter);
	std::vector<std::vector<Vector2f>> wreathMatrixPositions;
	std::vector<std::vector<lootItemsIdList>> wreathMatrix;
	std::vector<std::vector<std::pair<lootItemsIdList, int>>> plantsMatrix;
	std::unordered_map<lootItemsIdList, std::vector<lootItemsIdList>> plantsConnections;
	std::unordered_map<ButtonTag, WreathDraft> getOriginalSetups() { return originalSetups; };
};

