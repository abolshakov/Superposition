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

	std::string buttonImagePathDefault, buttonImagePathPressed, buttonImagePathSelected;
	Vector2f buttonPosition, buttonSize; // in percents
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);


	while (fin >> isSelectable)
	{
		if (isSelectable)
			fin >> buttonImagePathDefault >> buttonImagePathPressed >> buttonImagePathSelected;
		else
			fin >> buttonImagePathDefault;

		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> tag;

		Texture buttonTextureDefault, buttonTexturePressed, buttonTextureSelected;
		buttonTextureDefault.loadFromFile(buttonImagePathDefault);
		buttonTexturePressed.loadFromFile(buttonImagePathPressed);
		buttonTextureSelected.loadFromFile(buttonImagePathSelected);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = buttonTextureDefault.getSize().x * buttonSize.y / buttonTextureDefault.getSize().y;

		buttonList[ButtonTag(tag)].initialize(buttonTextureDefault, buttonTexturePressed, buttonTextureSelected, buttonPosition, buttonSize, isSelectable, ButtonTag(tag));
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

	//positioning interface elements
	buttonList.at(bookButtonTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(bookButtonTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(bookButtonTag).getGlobalBounds().height));
	buttonList.at(bookStandTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(bookStandTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(bookButtonTag).getGlobalBounds().height));
	buttonList.at(bookGlowTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(bookStandTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(bookButtonTag).getGlobalBounds().height));
	//------------------------------
}

void HeroBook::initContent()
{
	allContentChains.resize(1000, std::vector<std::vector<contentBlock>>(1000));
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

void HeroBook::drawHpLine(RenderWindow* window, float hpRatio)
{
	buttonList.at(hpFrameTag).setPosition(getHpLinePosition());
	buttonList.at(hpLineTag).setSize(Vector2f(hpRatio * buttonList.at(hpFrameTag).getGlobalBounds().width, buttonList.at(hpFrameTag).getGlobalBounds().height));
	buttonList.at(hpLineTag).setPosition(getHpLinePosition());

	buttonList.at(hpFrameTag).draw(*window);
	buttonList.at(hpLineTag).draw(*window);
}

void HeroBook::draw(RenderWindow* window, float hpRatio, float elapsedTime)
{
	drawHpLine(window, hpRatio);
	buttonList.at(bookStandTag).draw(*window);
	buttonList.at(bookButtonTag).draw(*window);
	buttonList.at(bookGlowTag).draw(*window);

	if (!visibility)
		return;

	FloatRect pageGlobalBounds;

	if (currentPage != 0)
	{
		buttonList.at(pageBackground).draw(*window);
		if (currentPage >= 901 && currentPage <= 905)
			buttonList.at(bookmarksList).draw(*window);
		else
			buttonList.at(pagePattern).draw(*window);

		pageGlobalBounds = buttonList.at(pageBackground).getGlobalBounds();
	}

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

	//draw arrows, bookmarks, cover, etc...
	buttonList.at(bookmarkMobs).draw(*window);
	buttonList.at(bookmarkItems).draw(*window);
	buttonList.at(bookmarkHerbs).draw(*window);
	buttonList.at(bookmarkWreathes).draw(*window);
	buttonList.at(bookmarkNightmare).draw(*window);

	if (currentPage == 0)
	{
		buttonList.at(bookCover).draw(*window);
		pageGlobalBounds = buttonList.at(bookCover).getGlobalBounds();
	}

	if (currentPage >= 999)
		buttonList.at(nextPage).isActive = false;
	else
	{
		buttonList.at(previousPage).isActive = true;
		buttonList.at(nextPage).draw(*window);
	}
	if (currentPage <= 0)
		buttonList.at(previousPage).isActive = false;
	else
	{
		buttonList.at(previousPage).isActive = true;
		buttonList.at(previousPage).draw(*window);
	}
	//------------------------------
}

void HeroBook::onMouseDown()
{
	if (buttonList.at(bookButtonTag).isSelected(Vector2f(Mouse::getPosition())))
		changeVisibility();
	if (buttonList.at(nextPage).isSelected(Vector2f(Mouse::getPosition())))
		setPage(currentPage + 1);
	if (buttonList.at(previousPage).isSelected(Vector2f(Mouse::getPosition())) && currentPage > 0)
		setPage(currentPage - 1);
	if (buttonList.at(bookmarkMobs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(901);
	if (buttonList.at(bookmarkItems).isSelected(Vector2f(Mouse::getPosition())))
		setPage(902);
	if (buttonList.at(bookmarkHerbs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(903);
	if (buttonList.at(bookmarkWreathes).isSelected(Vector2f(Mouse::getPosition())))
		setPage(904);
	if (buttonList.at(bookmarkNightmare).isSelected(Vector2f(Mouse::getPosition())))
		setPage(905);
}
