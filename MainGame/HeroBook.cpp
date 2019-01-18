#include <fstream>

#include "HeroBook.h"
#include "Helper.h"

HeroBook::HeroBook()
{
	initButtons();

	initContent();
	somePage.setContentChain(allContentChains);
}

HeroBook::~HeroBook()
{
}

void HeroBook::initButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

	std::string buttonImagePath;
	Vector2f buttonPosition, buttonSize; // in percents
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);
	while (fin >> buttonImagePath >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> isSelectable >> tag)
	{
		Texture buttonTexture;
		buttonTexture.loadFromFile(buttonImagePath);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = buttonTexture.getSize().x * buttonSize.y / buttonTexture.getSize().y;
		//buttonSize = Vector2f(buttonTexture.getSize());

		buttonList[ButtonTag(tag)].initialize(buttonTexture, buttonPosition, buttonSize, isSelectable, ButtonTag(tag));
	}

	fin.close();

	//bind all the buttons to page
	std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttons;
	for (auto it = buttonList.begin(); it != buttonList.end(); it++)
	{
		buttons.insert({ it->first, it->second });
	}
	somePage.buttonListBounding(buttons);
	//----------------------------
}

void HeroBook::initContent()
{
	allContentChains.resize(100, std::vector<std::vector<contentBlock>>(100));
	int pageNumber, blocksCount;

	std::ifstream fin(contentInfoFileDirectory);
	while (fin >> pageNumber >> blocksCount)
	{
		for (int cnt = 0; cnt < blocksCount; cnt++)
		{
			contentBlock block;
			int blockLevel, blockTag, connectionsCount;
			std::string blockDescription;

			fin >> blockLevel >> blockTag >> connectionsCount;
			std::vector<std::pair<int, int>> connections;
			if (connectionsCount > 0)
			{
				std::pair<int, int> connectedBlockPos;
				for (int i = 0; i < connectionsCount; i++)
				{
					fin >> connectedBlockPos.first >> connectedBlockPos.second;
					connections.push_back(connectedBlockPos);
				}
			}
			fin >> blockDescription;
			block.tag = ButtonTag(blockTag);
			block.connections = connections;
			block.textDescription = blockDescription;
			allContentChains[pageNumber][blockLevel - 1].push_back(block);
		}
	}

	fin.close();
}

void HeroBook::setPage(int page)
{
	currentPage = page;
	somePage.setPage(page);
}

void HeroBook::draw(RenderWindow* window, float elapsedTime)
{
	if (!boundedWorld->getHeroBookVisability())
		return;

	FloatRect pageGlobalBounds;

	if (currentPage == 0)
	{
		buttonList.at(bookCover).draw(*window);
		pageGlobalBounds = buttonList.at(bookCover).getGlobalBounds();
	}
	else
	{
		buttonList.at(pageBackground).draw(*window);
		pageGlobalBounds = buttonList.at(pageBackground).getGlobalBounds();
	}

	//draw arrows, bookmarks, etc...
	if (currentPage < 100)
		buttonList.at(nextPage).draw(*window);
	if (currentPage > 0)
		buttonList.at(previousPage).draw(*window);

	buttonList.at(bookmarkMobs).draw(*window);
	buttonList.at(bookmarkItems).draw(*window);
	buttonList.at(bookmarkHerbs).draw(*window);
	buttonList.at(bookmarkWreathes).draw(*window);
	buttonList.at(bookmarkNightmare).draw(*window);
	//------------------------------

	auto pageContent = somePage.getPreparedContent(currentPage);

	//main page content
	for (auto connection : pageContent.connections)
	{
		window->draw(connection);
	}

	for (auto item : pageContent.buttons)
	{
		buttonList[item].draw(*window);
	}

	textWriter.drawTextBox(pageContent.blockDescription, BebasFont, 40, pageGlobalBounds.left + pageGlobalBounds.width / 2, pageGlobalBounds.top, pageGlobalBounds.width / 2, pageGlobalBounds.height, window);
	//-----------------
}

void HeroBook::onMouseDown()
{
	if (buttonList.at(nextPage).isSelected(Vector2f(Mouse::getPosition())))
		setPage(currentPage + 1);
	if (buttonList.at(previousPage).isSelected(Vector2f(Mouse::getPosition())) && currentPage > 0)
		setPage(currentPage - 1);
	if (buttonList.at(bookmarkMobs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(1);
	if (buttonList.at(bookmarkItems).isSelected(Vector2f(Mouse::getPosition())))
		setPage(2);
	if (buttonList.at(bookmarkHerbs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(3);
	if (buttonList.at(bookmarkWreathes).isSelected(Vector2f(Mouse::getPosition())))
		setPage(4);
	if (buttonList.at(bookmarkNightmare).isSelected(Vector2f(Mouse::getPosition())))
		setPage(10);
}
