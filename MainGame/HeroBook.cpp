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

	// bind all the buttons to page
	std::unordered_map<ButtonTag, std::reference_wrapper<ButtonMaker>> buttons;
	for (auto it = buttonList.begin(); it != buttonList.end(); it++)
	{
		buttons.insert({ it->first, it->second });
	}
	somePage.buttonListBounding(buttons);
	//-----------------------------

	// positioning interface elements
	buttonList.at(ButtonTag::bookButtonTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookButtonTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).getGlobalBounds().height));
	buttonList.at(ButtonTag::bookStandTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).getGlobalBounds().height));
	buttonList.at(ButtonTag::bookGlowTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).getGlobalBounds().height));
	buttonList.at(ButtonTag::bookLightningTag).setPosition(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).getGlobalBounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).getGlobalBounds().height));
	//-------------------------------
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
	buttonList.at(ButtonTag::hpFrameTag).setPosition(getHpLinePosition());
	buttonList.at(ButtonTag::hpLineTag).setSize(Vector2f(hpRatio * buttonList.at(ButtonTag::hpFrameTag).getGlobalBounds().width, buttonList.at(ButtonTag::hpFrameTag).getGlobalBounds().height));
	buttonList.at(ButtonTag::hpLineTag).setPosition(getHpLinePosition());

	buttonList.at(ButtonTag::hpFrameTag).draw(*window);
	buttonList.at(ButtonTag::hpLineTag).draw(*window);
}

void HeroBook::drawWreathMatrix(RenderWindow* window, pageContent content)
{
	if (currentPage != 2)
		return;
	
	const Vector2f upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).getGlobalBounds().left + buttonList.at(ButtonTag::sketching).getGlobalBounds().width / 1.875,
		buttonList.at(ButtonTag::sketching).getGlobalBounds().top + buttonList.at(ButtonTag::sketching).getGlobalBounds().height / 8.2);
	Vector2f distance;
	float offset;
	ButtonTag currentType;

	for (int raw = 0; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (int column = 0; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] == lootItemsIdList::bagCellSelected)
				currentType = ButtonTag::cellSelected;
			else
				currentType = ButtonTag::cell;

			if (raw % 2 != 0)
				offset = buttonList.at(currentType).getGlobalBounds().width * 3 / 4;
			else
				offset = 0;

			distance.x = buttonList.at(currentType).getGlobalBounds().width * 6 / 4;
			distance.y = buttonList.at(currentType).getGlobalBounds().height / 2;
			somePage.wreathMatrixPositions[raw][column] = Vector2f(upperLeftCorner.x + column * distance.x + offset, upperLeftCorner.y + raw * distance.y);

			if (HeroBookPage::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).getOriginalSetup().rings))
			{				
				// draw cell content
				buttonList.at(currentType).setPosition(somePage.wreathMatrixPositions[raw][column]);
				buttonList.at(currentType).draw(*window);
				if (somePage.wreathMatrix[raw][column] != lootItemsIdList::bagCell && currentType == ButtonTag::cell)
				{
					const Vector2f contentOffset = Vector2f(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().width * 0.06,
						buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().height * 0.1);
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).setPosition(Vector2f(somePage.wreathMatrixPositions[raw][column].x - contentOffset.x, somePage.wreathMatrixPositions[raw][column].y - contentOffset.y));
					//for (const auto cell : somePage.getBorderCells(raw, column))
						//for (auto connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
							//if (somePage.wreathMatrix[cell.first][cell.second] == connection)
								buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).draw(*window);
				}
				//------------------
			}
		}
	}	
	buttonList.at(ButtonTag::emptyDraftCenter).setPosition(somePage.wreathMatrixPositions[8][2]);
	buttonList.at(ButtonTag::emptyDraftCenter).draw(*window);
	buttonList.at(currentDraft).setPosition(somePage.wreathMatrixPositions[8][2]);
	buttonList.at(currentDraft).draw(*window);
}

void HeroBook::drawPlantsMatrix(RenderWindow* window)
{
	if (currentPage != 2)
		return;

	const Vector2f upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).getGlobalBounds().left + buttonList.at(ButtonTag::sketching).getGlobalBounds().width * 0.066,
		buttonList.at(ButtonTag::sketching).getGlobalBounds().top + buttonList.at(ButtonTag::sketching).getGlobalBounds().height * 0.55);

	for (int raw = 0; raw < somePage.plantsMatrix.size(); raw++)
	{
		for (int collumn = 0; collumn < somePage.plantsMatrix[raw].size(); collumn++)
		{
			if (somePage.plantsMatrix[raw][collumn].first == lootItemsIdList::bagCell || somePage.plantsMatrix[raw][collumn].second == 0)
				continue;
			auto currentType = ButtonTag(somePage.plantsMatrix[raw][collumn].first);
			Vector2f size = Vector2f(buttonList.at(currentType).getGlobalBounds().width, buttonList.at(currentType).getGlobalBounds().height);

			buttonList.at(currentType).setPosition(Vector2f(upperLeftCorner.x + collumn * size.x, upperLeftCorner.y + raw * size.y));
			buttonList.at(currentType).draw(*window);
		}
	}
}

void HeroBook::draw(RenderWindow* window, float hpRatio, float elapsedTime)
{
	drawHpLine(window, hpRatio);
	buttonList.at(ButtonTag::bookStandTag).draw(*window);
	buttonList.at(ButtonTag::bookButtonTag).draw(*window);
	buttonList.at(ButtonTag::bookGlowTag).draw(*window);
	if (buttonList.at(ButtonTag::bookButtonTag).isSelected(Vector2f(Mouse::getPosition())))
		buttonList.at(ButtonTag::bookLightningTag).draw(*window);

	if (!visibility)
		return;

	FloatRect pageGlobalBounds;

	if (currentPage != 0)
	{
		buttonList.at(ButtonTag::pageBackground).draw(*window);		
		pageGlobalBounds = buttonList.at(ButtonTag::pageBackground).getGlobalBounds();
	}

	auto pageContent = somePage.getPreparedContent(currentPage, currentDraft);
	if (worldHeldItem != nullptr)
		this->debugInfo = std::to_string(int(worldHeldItem->content.first)) + " " + std::to_string(worldHeldItem->content.second);

	// main page content
	for (auto connection : pageContent.connections)
	{
		window->draw(connection);
	}

	for (auto item : pageContent.buttons)
	{
		buttonList[item].draw(*window);
	}

	drawWreathMatrix(window, pageContent);
	drawPlantsMatrix(window);

	textWriter.drawTextBox(pageContent.blockDescription, BebasFont, 40, pageGlobalBounds.left + pageGlobalBounds.width / 2, pageGlobalBounds.top, pageGlobalBounds.width / 2, pageGlobalBounds.height, window);
	//------------------

	//draw arrows, ButtonTag::bookmarks, cover, etc...
	buttonList.at(ButtonTag::bookmarkMobs).draw(*window);
	buttonList.at(ButtonTag::bookmarkItems).draw(*window);
	buttonList.at(ButtonTag::bookmarkHerbs).draw(*window);
	buttonList.at(ButtonTag::bookmarkWreathes).draw(*window);
	buttonList.at(ButtonTag::bookmarkNightmare).draw(*window);

	if (currentPage == 0)
		buttonList.at(ButtonTag::bookCover).draw(*window);			

	if (currentPage >= 999)
		buttonList.at(ButtonTag::nextPage).isActive = false;
	else
	{
		buttonList.at(ButtonTag::previousPage).isActive = true;
		buttonList.at(ButtonTag::nextPage).draw(*window);
	}
	if (currentPage <= 0)
		buttonList.at(ButtonTag::previousPage).isActive = false;
	else
	{
		buttonList.at(ButtonTag::previousPage).isActive = true;
		buttonList.at(ButtonTag::previousPage).draw(*window);
	}
	//------------------------------

	//draw held item
	//debugInfo = std::to_string(int(heldItem.first)) + " " + std::to_string(heldItem.second);
	if (heldItem.first != lootItemsIdList::bagCell)
	{
		const float offset = buttonList.at(ButtonTag(heldItem.first)).getGlobalBounds().width / 2;
		buttonList.at(ButtonTag(heldItem.first)).setPosition(Vector2f(Mouse::getPosition().x - offset, Mouse::getPosition().y - offset));
		buttonList.at(ButtonTag(heldItem.first)).draw(*window);
	}
	//--------------
}

void HeroBook::interact(float elapsedTime)
{
	if (Mouse::isButtonPressed(Mouse::Left))
		WhileMouseDown();
}

void HeroBook::onMouseUp()
{
	somePage.onMouseDown();

	// click on bookmark
	if (buttonList.at(ButtonTag::bookButtonTag).isSelected(Vector2f(Mouse::getPosition())))
		changeVisibility();
	if (buttonList.at(ButtonTag::nextPage).isSelected(Vector2f(Mouse::getPosition())))
		setPage(currentPage + 1);
	if (buttonList.at(ButtonTag::previousPage).isSelected(Vector2f(Mouse::getPosition())) && currentPage > 0)
		setPage(currentPage - 1);
	if (buttonList.at(ButtonTag::bookmarkMobs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(1);
	if (buttonList.at(ButtonTag::bookmarkItems).isSelected(Vector2f(Mouse::getPosition())))
		setPage(2);
	if (buttonList.at(ButtonTag::bookmarkHerbs).isSelected(Vector2f(Mouse::getPosition())))
		setPage(3);
	if (buttonList.at(ButtonTag::bookmarkWreathes).isSelected(Vector2f(Mouse::getPosition())))
		setPage(4);
	if (buttonList.at(ButtonTag::bookmarkNightmare).isSelected(Vector2f(Mouse::getPosition())))
		setPage(5);
	//------------------

	// plant dragging to draft
	const auto selectedCell = somePage.getSelectedWreathCell();
	if (selectedCell != std::make_pair(-1, -1))
		somePage.wreathMatrix[selectedCell.first][selectedCell.second] = heldItem.first;
	heldItem = { lootItemsIdList::bagCell, 0 };
	//------------------------

	// set draft in or out of scheme center
	if (worldHeldItem->content.first != lootItemsIdList::bagCell &&
		buttonList.at(ButtonTag::emptyDraftCenter).isSelected(Vector2f(Mouse::getPosition())) && buttonList.at(ButtonTag::emptyDraftCenter).isActive)
	{
		if (worldHeldItem->content.first == lootItemsIdList::someWreathDraft)
		{
			currentDraft = ButtonTag(worldHeldItem->content.first);
			worldHeldItem->content = { lootItemsIdList::bagCell, 0 };
		}
	}
	else
	if (buttonList.at(ButtonTag::emptyDraftCenter).isSelected(Vector2f(Mouse::getPosition())) && buttonList.at(ButtonTag::emptyDraftCenter).isActive &&
		currentDraft != ButtonTag::emptyDraftCenter && worldHeldItem->content.first == lootItemsIdList::bagCell)
	{
		worldHeldItem->content = { lootItemsIdList(currentDraft), 1 };
		currentDraft = ButtonTag::emptyDraftCenter;
	}
	//-----------------------------

}

void HeroBook::WhileMouseDown()
{
	if (heldItem.first == lootItemsIdList::bagCell)
	{
		const auto selectedCell = somePage.getSelectedPlantsCell();
		if (selectedCell != std::make_pair(-1, -1))		
			heldItem = somePage.plantsMatrix[selectedCell.first][selectedCell.second];
	}
}
