#include <fstream>

#include "HeroBook.h"
#include "Helper.h"

HeroBook::HeroBook()
{
	initButtons();
	initContent();
	somePage.initAuxiliarySpriteMap();
}

HeroBook::~HeroBook()
{
}

void HeroBook::initButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

	std::string buttonImagePathDefault, buttonImagePathPressed, buttonImagePathSelected;
	Vector2f buttonPosition, buttonSize; // in percents
	Vector2f offset = {0, 0};
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);


	while (fin >> isSelectable)
	{
		if (isSelectable)
			fin >> buttonImagePathDefault >> buttonImagePathPressed >> buttonImagePathSelected;
		else
			fin >> buttonImagePathDefault;

		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> offset.x >> offset.y >> tag;

		Texture buttonTextureDefault, buttonTexturePressed, buttonTextureSelected;
		buttonTextureDefault.loadFromFile(buttonImagePathDefault);
		buttonTexturePressed.loadFromFile(buttonImagePathPressed);
		buttonTextureSelected.loadFromFile(buttonImagePathSelected);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = buttonTextureDefault.getSize().x * buttonSize.y / buttonTextureDefault.getSize().y;

		buttonList[ButtonTag(tag)].initialize(buttonTextureDefault, buttonTexturePressed, buttonTextureSelected, buttonPosition, buttonSize, isSelectable, ButtonTag(tag), offset);
	}

	fin.close();

	// bind all the buttons to page
	somePage.buttonListBounding(&buttonList);
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
	somePage.initObjectInfo();
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
	if (currentPage != 5 || somePage.getOriginalSetups()[currentDraft].id != currentDraft)
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
			if (somePage.wreathMatrix[raw][column] == Tag::selectedCell)
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

			if (HeroBookPage::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
			{
				// draw cell
				buttonList.at(currentType).setPosition(somePage.wreathMatrixPositions[raw][column]);
				buttonList.at(currentType).draw(*window);				
				//------------------
			}
		}
	}
	drawLineMatrix(window);
}

void HeroBook::drawLineMatrix(RenderWindow* window)
{
	Vector2f distance = Vector2f(buttonList.at(ButtonTag::cell).getGlobalBounds().width * 6 / 4,
		distance.y = buttonList.at(ButtonTag::cell).getGlobalBounds().height / 2);
	for (int raw = 0; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (int column = 0; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] == Tag::emptyCell)
				continue;

			if (somePage.wreathMatrix[raw][column] != Tag::emptyCell)
			{
				const Vector2f contentOffset = Vector2f(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().width * 0.03,
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().height * 0.05);

				for (const auto cell : somePage.getBorderCells(raw, column))
					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
						{
							if (HeroBookPage::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings) && 
								HeroBookPage::checkWreathCellFit(cell.first, cell.second, somePage.getOriginalSetups().at(currentDraft).rings))
							{
								const Vector2f firstLnDot = Vector2f(somePage.wreathMatrixPositions[raw][column].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[raw][column].y + distance.y + contentOffset.y);
								const Vector2f secondLnDot = Vector2f(somePage.wreathMatrixPositions[cell.first][cell.second].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[cell.first][cell.second].y + distance.y + contentOffset.y);
								auto line = Helper::makeLine(firstLnDot, secondLnDot);
								window->draw(line);
							}
						}
			}
		}
	}
	drawPlantsMatrix(window);
}

void HeroBook::drawPlantsMatrix(RenderWindow* window)
{
	for (int raw = 0; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (int column = 0; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] != Tag::emptyCell && somePage.wreathMatrix[raw][column] != Tag::selectedCell)
			{
				const Vector2f contentOffset = Vector2f(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().width * 0.195,
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).getGlobalBounds().height * 0.195);
				buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).setPosition(Vector2f(somePage.wreathMatrixPositions[raw][column].x - contentOffset.x,
					somePage.wreathMatrixPositions[raw][column].y - contentOffset.y));
				buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).bekomeGray();

				for (const auto cell : somePage.getBorderCells(raw, column))
					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
						{
							buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).stopBeingGray();
							break;
						}
				if (HeroBookPage::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
				{
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).draw(*window);
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).stopBeingGray();
				}
			}
		}
	}
}

void HeroBook::drawPlantsList(RenderWindow* window)
{
	if (currentPage != 5)
		return;

	const Vector2f upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).getGlobalBounds().left + buttonList.at(ButtonTag::sketching).getGlobalBounds().width * 0.066,
		buttonList.at(ButtonTag::sketching).getGlobalBounds().top + buttonList.at(ButtonTag::sketching).getGlobalBounds().height * 0.55);

	for (int raw = 0; raw < somePage.plantsMatrix.size(); raw++)
	{
		for (int column = 0; column < somePage.plantsMatrix[raw].size(); column++)
		{
			if (somePage.plantsMatrix[raw][column].first == Tag::emptyCell/* || somePage.plantsMatrix[raw][column].second == 0*/)
				continue;
			auto curFlower = ButtonTag(somePage.plantsMatrix[raw][column].first);
			Vector2f size = Vector2f(buttonList.at(curFlower).getGlobalBounds().width, buttonList.at(curFlower).getGlobalBounds().height);			

			buttonList.at(curFlower).setPosition(Vector2f(upperLeftCorner.x + column * size.x, upperLeftCorner.y + raw * size.y));
			if (buttonList.at(curFlower).isSelected(Vector2f(Mouse::getPosition())) && currentFlower == Tag::emptyCell)
				this->currentFlower = Tag(int(curFlower));

			buttonList.at(curFlower).stopBeingGray();
			somePage.drawIconFrame(curFlower, window, 1);
			buttonList.at(curFlower).draw(*window);
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

	// turn off all book buttons
	for (auto& button : buttonList)
		if (button.first != ButtonTag::bookButtonTag)
			button.second.isActive = false;
	if (heldItem.first == Tag::emptyCell)
		currentFlower = Tag::emptyCell;
	//-----------------------------

	if (!visibility)
		return;

	FloatRect pageGlobalBounds;
	

	if (currentPage != 0)
	{
		buttonList.at(ButtonTag::pageBackground).draw(*window);		
		pageGlobalBounds = buttonList.at(ButtonTag::pagePattern).getGlobalBounds();
	}

	auto pageContent = somePage.getPreparedContent(currentPage, currentDraft);

	// main page content	
	if (currentPage >= 101 && currentPage <= 499)
		somePage.drawLines(window);

	somePage.drawAllIcons(pageContent, window);

	drawWreathMatrix(window, pageContent);
	drawPlantsList(window);
	if (currentFlower != Tag::emptyCell)
		somePage.drawConnectableFlowers(currentFlower, window);

	textWriter.drawTextBox(pageContent.blockDescription, NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * blockDescriptionPoint.x,
		pageGlobalBounds.top + pageGlobalBounds.height * blockDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, Color(100, 68, 34, 180));
	textWriter.drawTextBox(pageContent.pageDescription, NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * pageDescriptionPoint.x,
		pageGlobalBounds.top + pageGlobalBounds.height * pageDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, Color(100, 68, 34, 180));

	somePage.drawHeadingText(window);
	//------------------

	//draw arrows
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
	debugInfo = std::to_string((Mouse::getPosition().x - buttonList.at(ButtonTag::bookmarksList).getGlobalBounds().left) / buttonList.at(ButtonTag::bookmarksList).getGlobalBounds().width) + " " 
	+ std::to_string((Mouse::getPosition().y - buttonList.at(ButtonTag::bookmarksList).getGlobalBounds().top) / buttonList.at(ButtonTag::bookmarksList).getGlobalBounds().height);

	if (heldItem.first != Tag::emptyCell)
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
	// plant dragging to draft
	if (currentPage == 5)
	{
		const auto selectedCell = somePage.getSelectedWreathCell();
		if (selectedCell != std::make_pair(-1, -1) && currentDraft != Tag::emptyDraft)
		{
			bool canBePlaced = true;
			auto plants = somePage.getOriginalSetups().at(currentDraft).plants;
			for (auto& plant : plants)
			{
				if (selectedCell.first == int(plant.second.first) && selectedCell.second == (plant.second.second) ||
					!HeroBookPage::checkWreathCellFit(selectedCell.first, selectedCell.second, somePage.getOriginalSetups().at(currentDraft).rings))
				{
					canBePlaced = false;
					break;
				}
			}
			if (canBePlaced)
				somePage.wreathMatrix[selectedCell.first][selectedCell.second] = heldItem.first;
		}
		somePage.readyToFinishDraft = somePage.isCenterSurrounded();
		if (heldItem.first != Tag::emptyCell)
		{
			heldItem = { Tag::emptyCell, 0 };
			return;
		}
	}
	//------------------------

	// change page
	if (buttonList.at(ButtonTag::bookButtonTag).isSelected(Vector2f(Mouse::getPosition())))
		changeVisibility();
	
	for (auto& button : buttonList)
		if (somePage.buttonToPage(button.first) != -1 && button.second.isSelected(Vector2f(Mouse::getPosition())) && button.second.isActive)
			setPage(somePage.buttonToPage(button.first));
	//------------	

	// clicking the plus in the draft center
	if (buttonList.at(ButtonTag::plus).isActive)
	{
		if (buttonList.at(ButtonTag::plus).isSelected(Vector2f(Mouse::getPosition())))
		{
			somePage.unlockObject(currentDraft);
			setPage(somePage.getHeadingPage(currentDraft));
			somePage.clearWreathMatrix();
			currentDraft = Tag::emptyDraft;
			somePage.readyToFinishDraft = false;
			
		}
	}
	//--------------------------------------

	// set draft into scheme center
	if (worldSelectedObject != nullptr)
	{
		const auto worldObjectTag = worldSelectedObject->tag;
		if (somePage.getOriginalSetups()[currentDraft].id == currentDraft)
		{
			if (worldMouseName == "Sketch")
			{
				visibility = true;
				setPage(5);
				currentDraft = worldObjectTag;
				somePage.setPlantsOnMatrix(somePage.getOriginalSetups().at(worldObjectTag).plants);
			}
		}
	}
	//-----------------------------

}

void HeroBook::WhileMouseDown()
{
	// flower to matrix transfer
	if (currentPage == 5) 
	{
		if (heldItem.first == Tag::emptyCell)
		{
			const auto selectedCell = somePage.getSelectedPlantsCell();
			if (selectedCell != std::make_pair(-1, -1))
			{
				heldItem.first = Tag(int(somePage.plantsMatrix[selectedCell.first][selectedCell.second].first));
				heldItem.second = somePage.plantsMatrix[selectedCell.first][selectedCell.second].second;
			}
		}
		else
		{
			if (int(heldItem.first) >= 401 && int(heldItem.first) <= 499)
				this->currentFlower = Tag(int(heldItem.first));
		}
	}
	//--------------------------
}

