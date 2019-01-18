#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <fstream>

#include "ButtonMaker.h"


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
	int div_up(int n, int d) { return n / d + (((n < 0) ^ (d > 0)) && (n % d)); }
	std::vector<std::vector<std::vector<contentBlock>>> allContentChains;
	std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList;
	void setBookmarkPosition();

	int currentPage = 2;
public:
	HeroBookPage();
	~HeroBookPage();
	void buttonListBounding(std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList);
	void setContentChain(std::vector<std::vector<std::vector<contentBlock>>> allContentChains);
	void setPage(int page) { this->currentPage = page; }

	pageContent getPreparedContent(int pageNumber);
};

