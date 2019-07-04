#include "HeroBookPage.h"
#include "Helper.h"


HeroBookPage::HeroBookPage()
{
	initDrafts();

	wreathMatrix.resize(17, std::vector<lootItemsIdList>(5));
	wreathMatrixPositions.resize(17, std::vector<Vector2f>(5));
	plantsMatrix.resize(2, std::vector<std::pair<lootItemsIdList, int>>(3));
	for (auto& raw : wreathMatrix)
		for (auto& cell : raw)
			cell = lootItemsIdList::bagCell;

	plantsMatrix[0][0] = { lootItemsIdList::chamomileFlower, 1 };
	plantsMatrix[0][1] = { lootItemsIdList::yarrowFlower, 1 };
	plantsMatrix[0][2] = { lootItemsIdList::fernFlower, 1 };
	plantsMatrix[1][0] = { lootItemsIdList::mugwortFlower, 1 };
	plantsMatrix[1][1] = { lootItemsIdList::poppyFlower, 1 };
	plantsMatrix[1][2] = { lootItemsIdList::bagCell, 0 };

	plantsConnections[lootItemsIdList::chamomileFlower] = { lootItemsIdList::poppyFlower, lootItemsIdList::yarrowFlower };
	plantsConnections[lootItemsIdList::yarrowFlower] = { lootItemsIdList::chamomileFlower, lootItemsIdList::fernFlower };
	plantsConnections[lootItemsIdList::fernFlower] = { lootItemsIdList::yarrowFlower, lootItemsIdList::mugwortFlower };
	plantsConnections[lootItemsIdList::mugwortFlower] = { lootItemsIdList::fernFlower, lootItemsIdList::poppyFlower };
	plantsConnections[lootItemsIdList::poppyFlower] = { lootItemsIdList::mugwortFlower, lootItemsIdList::chamomileFlower };
}

HeroBookPage::~HeroBookPage()
{
}

void HeroBookPage::initDrafts()
{
	int draftId, ringsCount, plantsCount;
	std::ifstream fin("Game/heroBook/drafts.txt");
	DraftInfo draftInfo;
	while (fin >> draftId >> ringsCount >> plantsCount)
	{
		originalSetups[ButtonTag(draftId)] = *(new WreathDraft());
		draftInfo.id = ButtonTag(draftId);

		for (int i = 0; i < ringsCount; i++)
		{
			int ring;
			fin >> ring;
			draftInfo.rings.push_back(ring);
		}

		for (int i = 0; i < plantsCount; i++)
		{
			int raw, column, plant;
			fin >> raw >> column >> plant;
			draftInfo.plants.push_back(std::make_pair(lootItemsIdList(plant), std::make_pair(raw, column)));
		}

		originalSetups[ButtonTag(draftId)].init(draftInfo);
	}

	fin.close();
}

void HeroBookPage::setContentChain(std::vector<std::vector<std::vector<contentBlock>>> allContentChains)
{
	this->allContentChains = allContentChains;
}

void HeroBookPage::setBookmarkPosition()
{
	FloatRect pageGlobalBounds, bookmarkGlobalBounds = buttonList.at(ButtonTag::bookmarkMobs).get().getGlobalBounds();
	if (currentPage == 0)
		pageGlobalBounds = buttonList.at(ButtonTag::bookCover).get().getGlobalBounds();
	else
		pageGlobalBounds = buttonList.at(ButtonTag::pageBackground).get().getGlobalBounds();

	float bookmarkApproximateX;
	if (currentPage == 0)
		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 8.9 / 10;
	else
		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 9.38 / 10;
	buttonList.at(ButtonTag::bookmarkMobs).get().setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 1.2 / 10));
	buttonList.at(ButtonTag::bookmarkItems).get().setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 2.2 / 10));
	buttonList.at(ButtonTag::bookmarkHerbs).get().setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 3.2 / 10));
	buttonList.at(ButtonTag::bookmarkWreathes).get().setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 4.2 / 10));
	buttonList.at(ButtonTag::bookmarkNightmare).get().setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 7.2 / 10));
}

void HeroBookPage::preparePageBase()
{
	buttonList.at(ButtonTag::pageBackground).get().setPosition(Vector2f(buttonList.at(ButtonTag::bookCover).get().getPosition().x - buttonList.at(ButtonTag::bookCover).get().getGlobalBounds().width,
		buttonList.at(ButtonTag::bookCover).get().getPosition().y));

	if (currentPage == 0)
		pageGlobalBounds = buttonList.at(ButtonTag::bookCover).get().getGlobalBounds();
	else
		pageGlobalBounds = buttonList.at(ButtonTag::pageBackground).get().getGlobalBounds();

	buttonList.at(ButtonTag::pagePattern).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));
	buttonList.at(ButtonTag::pagePattern).get().setSize(Vector2f(pageGlobalBounds.width * 4.23 / 5, pageGlobalBounds.height * 4.1 / 5));
	buttonList.at(ButtonTag::bookmarksList).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));
	buttonList.at(ButtonTag::bookmarksList).get().setSize(Vector2f(pageGlobalBounds.width * 4.23 / 5, pageGlobalBounds.height * 4.1 / 5));
	buttonList.at(ButtonTag::sketching).get().setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));
	buttonList.at(ButtonTag::sketching).get().setSize(Vector2f(pageGlobalBounds.width * 4.23 / 5, pageGlobalBounds.height * 4.1 / 5));

	blockGlobalBounds = buttonList.at(ButtonTag::yarrowBlockTag).get().getGlobalBounds();
	arrowToPageGlobalBounds = buttonList.at(ButtonTag::nextPage).get().getGlobalBounds();
	leftPagePosition = Vector2f(pageGlobalBounds.left, pageGlobalBounds.top);
	rightPagePosition = Vector2f(pageGlobalBounds.left + pageGlobalBounds.width / 2, pageGlobalBounds.top);

	const FloatRect pagePatternGlobalBounds = buttonList.at(ButtonTag::pagePattern).get().getGlobalBounds();

	buttonList.at(ButtonTag::nextPage).get().setPosition(Vector2f(pagePatternGlobalBounds.left + pagePatternGlobalBounds.width - arrowToPageGlobalBounds.width,
		pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height));
	buttonList.at(ButtonTag::previousPage).get().setPosition(Vector2f(pagePatternGlobalBounds.left,
		pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height));
}

void HeroBookPage::putBlocksToPositions(std::vector<ButtonTag> buttons)
{
	const Vector2f upperLeftCorner = buttonList.at(ButtonTag::bookmarksList).get().getPosition();
	float x = buttonList.at(ButtonTag::bookmarksList).get().getGlobalBounds().width, y = buttonList.at(ButtonTag::bookmarksList).get().getGlobalBounds().height;
	std::vector<Vector2f> positions = { {0.084, 0.344}, {0.084, 0.474}, {0.084, 0.603}, {0.084, 0.724}, {0.545, 0.250}, {0.545, 0.379}, {0.545, 0.508}, {0.545, 0.629}, {0.545, 0.758} };
	for (int i = 0; i < buttons.size(); i++)	
		buttonList.at(buttons[i]).get().setPosition(Vector2f(positions[i % positions.size()].x * x + upperLeftCorner.x, positions[i % positions.size()].y * y + upperLeftCorner.y));
}

bool HeroBookPage::checkWreathCellFit(int i, int j, std::vector<int> rings)
{
	//all rings
	if ((i == 0 || i == 16) && (j == 0 || j == 1 || j == 3 || j == 4))
		return false;
	if ((i == 1 || i == 15) && (j == 0 || j == 3 || j == 4))
		return false;
	if ((i == 2 || i == 14) && (j == 0 || j == 4))
		return false;
	if ((i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13) && (j == 4))
		return false;
	if (i == 8 && j == 2)
		return false;
	//---------

	//if (rings.empty())
		//return true;

	std::vector<int> invertedRings = { 1, 2, 3, 4 };
	for (auto ring : rings)
	{
		for (int cnt = 0; cnt < invertedRings.size(); cnt++)
			if (ring == invertedRings[cnt])
				invertedRings.erase(invertedRings.begin() + cnt);
	}

	std::sort(invertedRings.begin(), invertedRings.end());

	for (int invertedRing : invertedRings)
	{
		switch (invertedRing)
		{
		case 1:
			if ((i == 4 && j == 0) || (i == 3 && j == 0) || (i == 2 && j == 1) || (i == 1 && j == 1) || (i == 0 && j == 2) || (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 3 && j == 3) ||
				(i == 4 && j == 4) || (i == 4 && j == 4) || (i == 6 && j == 4) || (i == 8 && j == 4) || (i == 10 && j == 4) || (i == 12 && j == 4) ||
				(i == 13 && j == 3) || (i == 14 && j == 3) || (i == 15 && j == 2) || (i == 16 && j == 2) || (i == 15 && j == 1) || (i == 14 && j == 1) || (i == 13 && j == 0) || (i == 12 && j == 0) ||
				(i == 10 && j == 0) || (i == 8 && j == 0) || (i == 6 && j == 0))
				return false;
			break;
		case 2:
			if ((i == 5 && j == 0) || (i == 4 && j == 1) || (i == 3 && j == 1) || (i == 2 && j == 2) || (i == 3 && j == 2) || (i == 4 && j == 3) || (i == 5 && j == 3) || (i == 7 && j == 3) || (i == 9 && j == 3) ||
				(i == 11 && j == 3) || (i == 12 && j == 3) || (i == 13 && j == 2) || (i == 14 && j == 2) || (i == 13 && j == 1) || (i == 12 && j == 1) || (i == 11 && j == 0) || (i == 9 && j == 0) || (i == 7 && j == 0))
				return false;
			break;
		case 3:
			if ((i == 6 && j == 1) || (i == 5 && j == 1) || (i == 4 && j == 2) || (i == 5 && j == 2) || (i == 6 && j == 3) || (i == 8 && j == 3) || (i == 10 && j == 3) || (i == 11 && j == 2) || (i == 12 && j == 2) ||
				(i == 11 && j == 1) || (i == 10 && j == 1) || (i == 8 && j == 1))
				return false;
			break;
		case 4:
			if ((i == 7 && j == 1) || (i == 6 && j == 2) || (i == 7 && j == 2) || (i == 9 && j == 2) || (i == 10 && j == 2) || (i == 9 && j == 1))
				return false;
			break;
		}
	}
	return true;
}

void HeroBookPage::onMouseDown()
{
	switch (currentPage)
	{
	case 2:

		break;
	default:
		break;
	}	
}

std::pair<int, int> HeroBookPage::getSelectedWreathCell()
{
	Vector2f mousePos = Vector2f(Mouse::getPosition());
	float minD = 10e4;
	std::pair<int, int> ans = std::make_pair(-1, -1);
	for (int cnt1 = 0; cnt1 < wreathMatrix.size(); cnt1++)
		for (int cnt2 = 0; cnt2 < wreathMatrix[cnt1].size(); cnt2++)
		{
			if (wreathMatrix[cnt1][cnt2] == lootItemsIdList::bagCellSelected)
				wreathMatrix[cnt1][cnt2] = lootItemsIdList::bagCell;
			Vector2f curPos = Vector2f(wreathMatrixPositions[cnt1][cnt2].x + buttonList.at(ButtonTag::cell).get().getGlobalBounds().width / 2,
				wreathMatrixPositions[cnt1][cnt2].y + buttonList.at(ButtonTag::cell).get().getGlobalBounds().height / 2);
			if (Helper::getDist(Vector2f(mousePos), curPos) < minD &&
				Helper::getDist(Vector2f(mousePos), curPos) <= buttonList.at(ButtonTag::cell).get().getGlobalBounds().width / 2)
			{
				minD = Helper::getDist(Vector2f(mousePos), curPos);
				ans = std::make_pair(cnt1, cnt2);
			}
		}

	return ans;
}

std::pair<int, int> HeroBookPage::getSelectedPlantsCell()
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).get().getGlobalBounds().left + buttonList.at(ButtonTag::sketching).get().getGlobalBounds().width * 0.066,
		buttonList.at(ButtonTag::sketching).get().getGlobalBounds().top + buttonList.at(ButtonTag::sketching).get().getGlobalBounds().height * 0.55);

	const Vector2f size = Vector2f(buttonList.at(ButtonTag::chamomile).get().getGlobalBounds().width,
		buttonList.at(ButtonTag::chamomile).get().getGlobalBounds().height);

	if (mousePos.x < upperLeftCorner.x || mousePos.y < upperLeftCorner.y ||
		mousePos.x > upperLeftCorner.x + size.x * plantsMatrix[0].size() || mousePos.y > upperLeftCorner.y + size.y * plantsMatrix.size())
		return { -1, -1 };

	std::pair<int, int> ans = std::make_pair((mousePos.y - upperLeftCorner.y) / size.y,
	                                         (mousePos.x - upperLeftCorner.x) / size.x);

	if (plantsMatrix[ans.first][ans.second].first == lootItemsIdList::bagCell || plantsMatrix[ans.first][ans.second].second == 0)
		return { -1, -1 };

	return ans;
}

std::vector<std::pair<int, int>> HeroBookPage::getBorderCells(int raw, int column)
{
	std::vector<std::pair<int, int>> ans;
	if (raw == -1 || column == -1)
		return {};
	if (raw % 2 == 0)	
		ans = { {raw - 2, column}, {raw - 1, column}, {raw + 1, column}, {raw + 2, column}, {raw + 1, column - 1}, {raw - 1, column - 1} };
	else
		ans = { {raw - 2, column}, {raw - 1, column + 1}, {raw + 1, column + 1}, {raw + 2, column}, {raw + 1, column}, {raw - 1, column} };

	while (true)
	{
		bool isBreak = true;
		for (int i = 0; i < ans.size(); i++)
			if (ans[i].first < 0 || ans[i].second < 0 || ans[i].first >= wreathMatrix.size() || ans[i].second >= wreathMatrix[0].size())
			{
				ans.erase(ans.begin() + i);
				isBreak = false;
			}
		if (isBreak)
			break;
	}

	return ans;
}

pageContent HeroBookPage::getPreparedContent(int pageNumber, ButtonTag currentDraft)
{
	pageContent result;

	preparePageBase();

	switch (currentPage)
	{
	case 1:
	{
		std::vector<ButtonTag> blocks = { ButtonTag::hareBlockTag, ButtonTag::owlBlockTag, ButtonTag::deerBlockTag, ButtonTag::bearBlockTag };
		putBlocksToPositions(blocks);
		result.buttons.insert(result.buttons.begin(), blocks.begin(), blocks.end());
		result.buttons.push_back(ButtonTag::bookmarksList);		
	}
	break;
	case 2:
	{		
		// placing plants according to the draft
		if (currentDraft == ButtonTag::emptyDraftCenter)
		{
			// clean wreath matrix
			for (auto& raw : wreathMatrix)
				for (auto& column : raw)
					column = lootItemsIdList::bagCell;
			//--------------------
		}
		for (const auto plant : originalSetups[currentDraft].getOriginalSetup().plants)
			wreathMatrix[plant.second.first][plant.second.second] = plant.first;
		//-------------------------------------

		// select cell
		const auto selectedCell = getSelectedWreathCell();
		if (selectedCell != std::make_pair(-1, -1) && wreathMatrix[selectedCell.first][selectedCell.second] == lootItemsIdList::bagCell)
			wreathMatrix[selectedCell.first][selectedCell.second] = lootItemsIdList::bagCellSelected;
		//------------

		result.buttons.push_back(ButtonTag::sketching);		
	}
	break;
	default:
		result.buttons.push_back(ButtonTag::pagePattern);
		// draw an icon chain of object
		if (!allContentChains[pageNumber].empty())
		{
			for (int i = 0; i < allContentChains[pageNumber].size(); i++)
			{
				const int blocksCount = allContentChains[pageNumber][i].size();

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

					if (allContentChains[pageNumber][i][j].connections.empty())
						continue;
					for (const auto connection : allContentChains[pageNumber][i][j].connections)
					{
						const Vector2f destinationPosition = buttonList.at(allContentChains[pageNumber][connection.first - 1][connection.second - 1].tag).get().getPosition();
						result.connections.push_back(Helper::makeLine(Vector2f(leftPagePosition.x + positionOffset.x + blockGlobalBounds.width / 2, leftPagePosition.y + positionOffset.y + blockGlobalBounds.height / 2),
							Vector2f(destinationPosition.x + blockGlobalBounds.width / 2, destinationPosition.y + blockGlobalBounds.height / 2)));
					}
				}
			}
		}
		break;
		//-----------------------------
	}

	setBookmarkPosition();

	return  result;
}

void HeroBookPage::buttonListBounding(std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttonList)
{
	this->buttonList = buttonList;
}


