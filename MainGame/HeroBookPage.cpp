#include "HeroBookPage.h"
#include "Helper.h"


HeroBookPage::HeroBookPage()
{
}


HeroBookPage::~HeroBookPage()
{
}

void HeroBookPage::setContentChain(std::vector<std::vector<std::vector<contentBlock>>> allContentChains)
{
	this->allContentChains = allContentChains;
}

void HeroBookPage::setBookmarkPosition()
{
	FloatRect pageGlobalBounds, bookmarkGlobalBounds = buttonList.at(bookmarkMobs).get().getGlobalBounds();

	if (currentPage == 0)
		pageGlobalBounds = buttonList.at(bookCover).get().getGlobalBounds();
	else
		pageGlobalBounds = buttonList.at(pageBackground).get().getGlobalBounds();

	buttonList.at(bookmarkMobs).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - 10, pageGlobalBounds.top));
	buttonList.at(bookmarkItems).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - 10, pageGlobalBounds.top + bookmarkGlobalBounds.height + 10));
	buttonList.at(bookmarkHerbs).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - 10, pageGlobalBounds.top + (bookmarkGlobalBounds.height + 10) * 2));
	buttonList.at(bookmarkWreathes).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - 10, pageGlobalBounds.top + (bookmarkGlobalBounds.height + 10) * 3));
	buttonList.at(bookmarkNightmare).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - 10, pageGlobalBounds.top + pageGlobalBounds.height - (bookmarkGlobalBounds.height + 10)));
}


pageContent HeroBookPage::getPreparedContent(int pageNumber)
{
	pageContent result;

	buttonList.at(pageBackground).get().setPosition(Vector2f(buttonList.at(bookCover).get().getPosition().x - buttonList.at(bookCover).get().getGlobalBounds().width, buttonList.at(bookCover).get().getPosition().y));

	FloatRect pageGlobalBounds;

	if (currentPage == 0)
		pageGlobalBounds = buttonList.at(bookCover).get().getGlobalBounds();
	else
		pageGlobalBounds = buttonList.at(pageBackground).get().getGlobalBounds();

	FloatRect blockGlobalBounds = buttonList.at(yarrowBlockTag).get().getGlobalBounds(),
		arrowToPageGlobalBounds = buttonList.at(nextPage).get().getGlobalBounds();
	Vector2f leftPagePosition = Vector2f(pageGlobalBounds.left, pageGlobalBounds.top),
		rightPagePosition = Vector2f(pageGlobalBounds.left + pageGlobalBounds.width / 2, pageGlobalBounds.top);

	buttonList.at(nextPage).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width - arrowToPageGlobalBounds.width,
		pageGlobalBounds.top + pageGlobalBounds.height - arrowToPageGlobalBounds.height));
	buttonList.at(previousPage).get().setPosition(Vector2f(pageGlobalBounds.left,
		pageGlobalBounds.top + pageGlobalBounds.height - arrowToPageGlobalBounds.height));

	if (!allContentChains[pageNumber].empty())
	{
		for (int i = 0; i < allContentChains[pageNumber].size(); i++)
		{
			int blocksCount = allContentChains[pageNumber][i].size();

			if (blocksCount == 0)
				continue;

			for (int j = 0; j < blocksCount; j++)
			{
				Vector2f positionOffset;
				const float pageWidth = pageGlobalBounds.width / 2;
				if (blocksCount % 2 != 0)					
					positionOffset = Vector2f(pageWidth / 2 + (j + 1 - div_up(blocksCount, 2)) * (pageWidth / (blocksCount + 1)) - blockGlobalBounds.width / 2, i * blockGlobalBounds.height * 1.5);
				else
					positionOffset = Vector2f(pageWidth / 2 + (j - div_up(blocksCount, 2)) * (pageWidth / (blocksCount + 1)) - blockGlobalBounds.width / 2 + pageWidth / (blocksCount + 1) / 2, i * blockGlobalBounds.height * 1.5);

				positionOffset.y += blockGlobalBounds.height;
				buttonList.at(allContentChains[pageNumber][i][j].tag).get().setPosition(Vector2f(leftPagePosition.x + positionOffset.x, leftPagePosition.y + positionOffset.y));
				
				result.buttons.push_back(allContentChains[pageNumber][i][j].tag);

				if (buttonList.at(allContentChains[pageNumber][i][j].tag).get().isSelected(Vector2f(Mouse::getPosition())))
					result.blockDescription = allContentChains[pageNumber][i][j].textDescription;

				if (allContentChains[pageNumber][i][j].connections.size() == 0)
					continue;
				for (auto connection : allContentChains[pageNumber][i][j].connections)
				{					
					Vector2f destinationPosition = buttonList.at(allContentChains[pageNumber][connection.first - 1][connection.second - 1].tag).get().getPosition();
					result.connections.push_back(Helper::makeLine(Vector2f(leftPagePosition.x + positionOffset.x + blockGlobalBounds.width / 2, leftPagePosition.y + positionOffset.y + blockGlobalBounds.height / 2), 
						Vector2f(destinationPosition.x + blockGlobalBounds.width / 2, destinationPosition.y + blockGlobalBounds.height / 2)));
				}

				
			}
		}
	}

	setBookmarkPosition();

	return  result;
}

void HeroBookPage::buttonListBounding(std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList)
{
	this->buttonList = buttonList;
}


