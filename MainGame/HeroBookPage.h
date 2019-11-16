#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <fstream>

#include "WreathDraft.h"
#include "SpriteStructures.h"
#include "TextWriter.h"
#include "HeroBag.h"

enum class AuxiliarySpriteTag { line1 = 1, line2 = 2, line3 = 3, line4 = 4, line5 = 5, line6 = 6, line7 = 7 };

struct Connection
{
	Connection (int id, int pedestal, std::string description)
	{
		this->id = id;
		this->pedestal = pedestal;
		this->description = description;
	}
	int id;
	int pedestal;
	std::string description;
};

struct BookObjectInfo
{
	Tag tag;
	bool isUnlocked = false;
	std::vector<Connection> connections;
	std::string textDescription;
};

struct pageContent
{
	std::vector<ButtonTag> buttons;
	std::string blockDescription;
	std::string pageDescription;
	std::vector<RectangleShape> connections;
};

class HeroBookPage
{
public:
	HeroBookPage();
	~HeroBookPage();
	void initAuxiliarySpriteMap();
	void buttonListBounding(std::unordered_map<ButtonTag, ButtonMaker>* buttonList);
	void setPage(int page) { this->currentPage = page; }
	void onMouseDown();

	// object information (creatures, craft, plants, wreaths, nightmare)
	void initObjectInfo();
	Tag pageToObjectId(int page);
	int buttonToPage(ButtonTag button);
	void setButtonLock(ButtonTag button, ButtonTag changedButton);
	void drawIconFrame(ButtonTag button, RenderWindow* window, int type = 1);
	void drawAllIcons(pageContent content, RenderWindow* window);
	void drawLines(RenderWindow* window);
	void drawHeadingText(RenderWindow* window);
	void unlockObject(Tag object);
	static int getHeadingPage(Tag object);
	std::unordered_map<Tag, BookObjectInfo>* getObjectInfo() { return &objectInfo; }
	static std::string buttonToString(ButtonTag button);
	static Tag tagToWreath(Tag item);
	//------------------------------------------------------------------

	//wreaths
	void clearWreathMatrix();
	std::pair<int, int> getSelectedWreathCell();
	std::pair<int, int> getSelectedPlantsCell();
	std::vector<std::pair<int, int>> getBorderCells(int raw, int column);
	static bool checkWreathCellFit(int i, int j, std::vector<int> rings = std::vector<int>());
	void setPlantsOnMatrix(std::vector<std::pair<Tag, std::pair<int, int>>> plants);
	bool isCenterSurrounded();
	void drawConnectableFlowers(Tag currentFlower, RenderWindow* window);

	std::vector<std::vector<Vector2f>> wreathMatrixPositions;
	std::vector<std::vector<Tag>> wreathMatrix;
	std::vector<std::vector<std::pair<Tag, int>>> plantsMatrix;
	std::map<Tag, std::map<Tag, int>> doneRecipes;
	std::unordered_map<Tag, std::vector<Tag>> plantsConnections;
	std::unordered_map<Tag, DraftInfo> getOriginalSetups() { return originalSetups; };
	bool readyToFinishDraft = false; 
	bool nearTheTable = false;
	//-------

	std::string debugInfo = "";
	pageContent getPreparedContent(int pageNumber, Tag currentDraft = Tag::emptyDraft);
	std::vector<HeroBag>* boundBags;
private:
	static int div_up(int n, int d) { return n / d + (((n < 0) ^ (d > 0)) && (n % d)); }
	std::unordered_map<ButtonTag, ButtonMaker>* buttonList;
	void setBookmarkPosition();
	void preparePageBase();
	void putHeadingsToPositions(std::vector<ButtonTag> buttons);
	void setArrowsPosition();
	
	int currentPage = 0;
	FloatRect pageGlobalBounds;
	FloatRect blockGlobalBounds, arrowToPageGlobalBounds;
	Vector2f leftPagePosition, rightPagePosition;

	std::unordered_map<AuxiliarySpriteTag, BoardSprite> auxiliarySpriteMap;
	TextWriter textWriter;

	// object information (creatures, craft, plants, wreaths, nightmare)
	Vector2f getConnectionPosition(int numberInOrder);	

	std::unordered_map<Tag, BookObjectInfo> objectInfo;
	std::vector<Vector2f> connectionPedestals = { {0.264, 0.278}, {0.113, 0.390}, {0.145, 0.606}, {0.126, 0.789}, {0.242, 0.479}, {0.269, 0.718}, {0.412, 0.789}, {0.363, 0.566} };
	std::vector<Vector2f> headingPedestals = { {0.061, 0.315}, {0.061, 0.441}, {0.061, 0.568}, {0.061, 0.696}, {0.53, 0.097}, {0.53, 0.222}, {0.53, 0.348}, {0.53, 0.478}, {0.53, 0.614}, {0.53, 0.740} };
	std::vector<Vector2f> headingTextPedestals = { {0.170, 0.315}, {0.170, 0.441}, {0.170, 0.568}, {0.170, 0.696}, {0.637, 0.097}, {0.637, 0.222}, {0.637, 0.348}, {0.637, 0.478}, {0.637, 0.614}, {0.637, 0.740} };
	std::vector<Vector2f> flowerConnectionsPedestals = { {0.072, 0.342}, {0.160, 0.342}, {0.246, 0.342} };
	//------------------------------------------------------------------

	// wreaths
	void initDrafts();
	void coloredDfs(int raw, int column, int color, bool flowerPatency = false);	

	std::unordered_map<Tag, DraftInfo> originalSetups;
	std::vector<std::vector<int>> coloredMatrix;	
	//--------
};

