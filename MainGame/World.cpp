#define _USE_MATH_DEFINES

#include "World.h"
#include "Rock.h"
#include "Ground.h"
#include "GroundConnection.h"
#include "DroppedLoot.h"
#include "Grass.h"
#include "Spawn.h"
#include "BonefireOfInsight.h"
#include "HomeCosiness.h"
#include "MushroomStone.h"
#include "MushroomsOnStone.h"
#include "Chamomile.h"
#include "Yarrow.h"
#include "Mugwort.h"
#include "Brazier.h"
#include "HareTrap.h"
#include "Fence.h"
#include "Stump.h"

#include "Monster.h"
#include "Deerchant.h"
#include "Wolf.h"
#include "Hare.h"
#include "Deer.h"
#include "OwlBoss.h"
#include "Noose.h"

using namespace sf;

World::World(int width, int height) : focusedObject(nullptr)
{
	timeAfterSave = 60000000;
	timeForNewSave = 0;
	blockSize = Vector2i(1000, 1000); //conditionalSizeUnits
	microblockSize = Vector2i(100, 100);
	initSpriteMap();
	scaleFactor = getScaleFactor();

	this->width = width;
	this->height = height;

	initShaders();
	inventorySystem.init();
	buildSystem.Init(inventorySystem.getSpriteList());

	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)		
			biomeMatrix[i][j].biomeCell = mud;		
}

World::~World()
{
	/*staticGrid.~GridList();
	dynamicGrid.~GridList();
	inventorySystem.~InventoryMaker();
	buildSystem.~BuildSystemMaker();
	focusedObject->~DynamicObject();
	for (auto item : staticItems)
		item->~StaticObject();
	for (auto item : visibleItems)
		item->~WorldObject();*/
}

void World::initShaders()
{
	if (!sf::Shader::isAvailable())
	{
		std::cerr << "Shader are not available" << std::endl;
	}

	// Load shaders

	if (!spiritWorldShader.loadFromFile("Game/Shaders/water.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
	}

	if (!distortionMap.loadFromFile("Game/Shaders/noiseWater.png"))
	{
		sf::err() << "Error while loading distortion map" << std::endl;
	}
	// It is important to set repeated to true to enable scrolling upwards
	distortionMap.setRepeated(true);
	// Setting smooth to true lets us use small maps even on larger images
	distortionMap.setSmooth(true);

	// Set texture to the shader
	spiritWorldShader.setUniform("currentTexture", sf::Shader::CurrentTexture);
	spiritWorldShader.setUniform("distortionMapTexture", distortionMap);

	// Shader parameters
	float level = 0;
}

void World::setScaleFactor(int delta)
{
	if (delta == -1 && scaleFactor >= 0.9 * mainScale)
	{
		scaleFactor -= 0.01;
		scaleDecrease = -0.03;
	}
	else
		if (delta == 1 && scaleFactor <= 1.7 * mainScale)
		{
			scaleFactor += 0.01;
			scaleDecrease = 0.03;
		}

	if (scaleDecrease < 0 && scaleFactor <= 0.9 * mainScale)
		scaleFactor = 0.9 * mainScale;
	if (scaleDecrease > 0 && scaleFactor >= 1.7 * mainScale)
		scaleFactor = 1.7 * mainScale;
}

void World::scaleSmoothing()
{
	if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
	{
		if (scaleFactor != 0.9 * mainScale && scaleFactor != 1.7 * mainScale)
			scaleFactor += scaleDecrease;
		if (scaleDecrease < 0 && scaleFactor <= 0.9 * mainScale)
			scaleFactor = 0.9 * mainScale;
		if (scaleDecrease > 0 && scaleFactor >= 1.7 * mainScale)
			scaleFactor = 1.7 * mainScale;

		if (scaleDecrease < 0)
		{
			scaleDecrease += 0.001;
		}
		else
			if (scaleDecrease > 0)
			{
				scaleDecrease -= 0.001;
			}


		timeForScaleDecrease = 0;
	}
}

float World::getScaleFactor()
{
	auto heroHeight = Vector2f(spriteMap[heroTextureName].texture.getSize()).y;
	auto screenHeight = Helper::GetScreenSize().y;
	auto ratio = heroHeight / float(screenHeight);

	auto mainObject = Deerchant("loadInit", Vector2f(0, 0));
	mainObject.calculateTextureOffset();
	mainScale = screenHeight / (5 * mainObject.getConditionalSizeUnits().y);
	mainScale = round(mainScale * 100) / 100;
	return mainScale;
	//return 1;
}

bool cmpImgDraw(spriteChainElement first, spriteChainElement second)
{
	if (first.zCoord == second.zCoord)
	{
		if (first.position.y == second.position.y)
			return first.position.x < second.position.x;
		return first.position.y < second.position.y;
	}

	return first.zCoord < second.zCoord;
}

bool World::searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders)
{
	LPTSTR part;
	char tmp[MAX_PATH]; // временный массив
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	// сначала поиск внутри вложенных папок ...
	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");

		// если папки существуют, делаем поиск
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				// в каждой папке есть две папки с именами "." и ".."
				// и эти папки мы не трогаем

				// пропускаем папки "." и ".."
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если мы нашли папку
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					searchFiles(next, lpSearchFunc, 1);
				}
			} while (FindNextFile(hSearch, &wfd)); // ищем следующий файл

			FindClose(hSearch); // заканчиваем поиск
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE; // в противном случае выходим
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // если мы нашли файл
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(file, spriteMap);
		}
	while (FindNextFile(hSearch, &wfd)); // ищем следующий файл
	FindClose(hSearch); // заканчиваем поиск

	return TRUE;
}

void putImageToMap(LPCTSTR lpszFileName, std::unordered_map<std::string, BoardSprite> &spriteMap)
{
	//std::string filePath = std::to_string(*lpszFileName);
	std::string filePath = lpszFileName;
	filePath.erase(0, filePath.find("\\Game") + 1);
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	spriteMap.insert({ filePath, BoardSprite{} });
	auto sprite = &spriteMap[filePath];
	sprite->texture.loadFromFile(filePath);
	sprite->sprite.setTexture(sprite->texture);
}

void World::initSpriteMap()
{
	int objectsNumber;
	std::string name;

	searchFiles("Game/*.png", putImageToMap, 1);
}

void World::initLightSystem(RenderWindow &window)
{
	view = window.getDefaultView();

	contextSettings.antialiasingLevel = 8;// ¬ключить сглаживание.

	penumbraTexture.loadFromFile("data/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	pointLightTexture.loadFromFile("data/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	directionLightTexture.loadFromFile("data/directionLightTexture.jpg");
	directionLightTexture.setSmooth(true);

	unshadowShader.loadFromFile("data/unshadowShader.vert", "data/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("data/lightOverShapeShader.vert", "data/lightOverShapeShader.frag");
	ls.create(ltbl::rectFromBounds(Vector2f(-1000.0f, -1000.0f), Vector2f(1000.0f, 1000.0f)), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::shared_ptr<ltbl::LightPointEmission> light1 = std::make_shared<ltbl::LightPointEmission>();
	light1->_emissionSprite.setOrigin(Vector2f(directionLightTexture.getSize().x * 0.5f, directionLightTexture.getSize().y * 0.5f));
	light1->_emissionSprite.setTexture(directionLightTexture);
	light1->_emissionSprite.setScale(Vector2f(Helper::GetScreenSize().x * 1.125f / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y * 1.125f / light1->_emissionSprite.getTexture()->getSize().y));
	light1->_emissionSprite.setColor(commonWorldColorOutfill);
	light1->_sourceRadius = 12;
	ls.addLight(light1);

	brightner = std::make_shared<ltbl::LightPointEmission>();
	brightner->_emissionSprite.setOrigin(Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	brightner->_emissionSprite.setTexture(pointLightTexture);
	//brightner->_emissionSprite.setScale(spriteMap[heroTextureName].sprite.getGlobalBounds().width / brightner->_emissionSprite.getTexture()->getSize().x, spriteMap[heroTextureName].sprite.getGlobalBounds().height / brightner->_emissionSprite.getTexture()->getSize().y);
	brightner->_emissionSprite.setScale(Vector2f(float(Helper::GetScreenSize().x * 0.5f / brightner->_emissionSprite.getTexture()->getSize().x), float(Helper::GetScreenSize().y * 0.5f / brightner->_emissionSprite.getTexture()->getSize().y)));
	//brightner->_emissionSprite.setScale(Vector2f (Helper::GetScreenSize().x / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y / light1->_emissionSprite.getTexture()->getSize().y));
	brightner->_emissionSprite.setColor(Color(240, 220, 220));
	brightner->_sourceRadius = 12;
	//ls.addLight(brightner);

	ls._ambientColor = commonWorldColor;
	light1->_emissionSprite.setPosition(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2));
}

void World::renderLightSystem(View view, RenderWindow &window)
{
	ls.render(view, unshadowShader, lightOverShapeShader);
	Lsprite.setTexture(ls.getLightingTexture());
	lightRenderStates.blendMode = sf::BlendMultiply;
	window.draw(Lsprite, lightRenderStates);
}

void World::initializeStaticItem(StaticItemsIdList itemClass, Vector2f itemPosition, int itemType, std::string itemName, bool reliable, int count)
{
	StaticObject* item = nullptr;

	switch (itemClass)
	{
	case 1:
	{
		item = new ForestTree("item", Vector2f(0, 0), -1);
		break;
	}
	case 2:
	{
		item = new Grass("item", Vector2f(0, 0), -1);
		break;
	}
	case 3:
	{
		item = new Spawn("item", Vector2f(0, 0), -1);	
		break;
	}
	case 4:
	{
		item = new BonefireOfInsight("item", Vector2f(0, 0), -1);		
		break;
	}
	case 5:
	{
		item = new HomeCosiness("item", Vector2f(0, 0), -1);		
		break;
	}
	case 6:
	{
		item = new MushroomStone("item", Vector2f(0, 0), -1);		
	}
	case 7:
	{
		item = new MushroomsOnStone("item", Vector2f(0, 0), -1);		
		break;
	}
	case 11:
	{
		item = new Ground("item", Vector2f(0, 0), -1);		
		break;
	}
	case 12:
	{
		item = new GroundConnection("item", Vector2f(0, 0), -1);		
		break;
	}
	case 13:
	{
		item = new Chamomile("item", Vector2f(0, 0), -1);		
		break;
	}
	case 14:
	{
		item = new Brazier("item", Vector2f(0, 0), -1);		
		break;
	}
	case 15:
	{
		item = new Yarrow("item", Vector2f(0, 0), -1);		
		break;
	}
	case 16:
	{
		item = new HareTrap("item", Vector2f(0, 0), -1);		
		break;
	}
	case 17:
	{
		item = new Rock("item", Vector2f(0, 0), -1);		
		break;
	}
	case 18:
	{
		item = new Fence("item", Vector2f(0, 0), -1);		
		break;
	}
	case 19:
	{
		item = new Stump("item", Vector2f(0, 0), -1);		
		break;
	}
	case 20:
	{
		item = new DroppedLoot("item", Vector2f(0, 0), -1, count);		
		break;
	}
	case 41:
	{
		item = new Mugwort("item", Vector2f(0, 0), -1);		
		break;
	}
	default:
	{
		item = new Spawn("item", Vector2f(0, 0), -1);
		break;
	}
	}

	if (!reliable)
		if (staticGrid.isIntersectWithOthers(itemPosition, item->getRadius(), localTerrain) && staticGrid.getIndexByPoint(itemPosition.x, itemPosition.y) != 0 && !item->isBackground)
		{
			delete item;
			return;
		}

	int currentType = itemType == -1
		? currentType = rand() % item->getVarietyOfTypes() + 1
		: currentType = itemType;

	newNameId++;

	item->setPosition(Vector2f(itemPosition));
	item->setType(currentType);
	item->prepareSpriteNames(0);
	auto textureBounds = spriteMap[item->additionalSprites[0].path].sprite.getGlobalBounds();	
	auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);
	std::string name = itemName == ""
		? item->additionalSprites[0].path + "_" + std::to_string(newNameId)
		: itemName;
	item->setName(name);

	//for bias positioning
	auto spriteLeft = float((item->getPosition().x - cameraPosition.x - item->getTextureOffset().x) * scaleFactor + Helper::GetScreenSize().x / 2);
	auto spriteTop = float((item->getPosition().y - cameraPosition.y + (item->getConditionalSizeUnits().y - item->getTextureOffset().y)) * scaleFactor + Helper::GetScreenSize().y / 2);

	staticGrid.addItem(item, name, int(itemPosition.x), int(itemPosition.y));
}

void World::initializeDynamicItem(DynamicItemsIdList itemClass, Vector2f itemPosition, std::string itemName)
{
	DynamicObject* item;
	std::string nameOfImage;

	switch (itemClass)
	{
		case 1:
		{
			item = new Deerchant("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/hero/stand/down/1";
			focusedObject = item;
			cameraPosition = Vector2f(itemPosition);
			break;
		}
		case 2:
		{
			item = new Monster("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/hare/stand/down/1";
			break;
		}
		case 3:
		{
			item = new Wolf("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/wolf/stand/down/1";
			break;
		}
		case 4:
		{
			item = new Hare("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/hare/stand/down/1";
			break;
		}
	    case 5:
		{
			item = new Deer("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/deer/stand/down/1";
			break;
		}
		case 6:
		{
			item = new Noose("item", Vector2f(0, 0), focusedObject);
			nameOfImage = "Game/worldSprites/noose/nooseLoop/1";
			break;
		}
		case 20:
		{
			item = new OwlBoss("item", Vector2f(0, 0));
			nameOfImage = "Game/worldSprites/owlBoss/stand/down/1";
			break;
		}
		default:
		{
			item = new Monster("item", Vector2f(0, 0));
			nameOfImage = "Game/enemy/enemyF_0";
			break;
		}
	}

	nameOfImage += ".png";

	std::string name = itemName == ""
		? nameOfImage + std::to_string(dynamicGrid.getSize())
		: itemName;

	item->setName(name);
	item->setPosition(Vector2f(itemPosition));
	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);

	dynamicGrid.addItem(item, name, int(itemPosition.x), int(itemPosition.y));
}

void World::birthObjects()
{
	for (auto object : localTerrain)
	{
		auto birthStaticStack = object->getBirthObjects().first;
		auto birthDynamicStack = object->getBirthObjects().second;

		while (!birthStaticStack.empty())
		{
			initializeStaticItem(birthStaticStack.top().id, birthStaticStack.top().position, birthStaticStack.top().typeOfObject, "", 1, birthStaticStack.top().count);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			initializeDynamicItem(birthDynamicStack.top().id, birthDynamicStack.top().position, "");
			birthDynamicStack.pop();
		}
		object->clearBirthStack();
	}
}

void World::Load()
{
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize, microblockSize);

	//
	initializeStaticItem(tree, Vector2f(0, 0), 1, "_forestTree1", 1);
	initializeStaticItem(spawn, Vector2f(0, 0), 1, "_spawn1", 1);
	initializeStaticItem(grass, Vector2f(0, 0), 1, "_grass1", 1);
	initializeStaticItem(ground, Vector2f(0, 0), 1, "_ground1", 1);
	initializeStaticItem(groundConnection, Vector2f(0, 0), 1, "_groundConnection1", 1);
	initializeStaticItem(bonefireOfInsight, Vector2f(0, 0), 1, "_bonefireOfInsight1", 1);
	initializeStaticItem(homeCosiness, Vector2f(0, 0), 1, "_homeCosiness1", 1);
	initializeStaticItem(mushroomStone, Vector2f(0, 0), 1, "_mushroomStone1", 1);
	initializeStaticItem(mushroomsOnStone, Vector2f(0, 0), 1, "_mushroomsOnStone1", 1);
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "_chamomile1", 1);
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "_yarrow1", 1);
	initializeStaticItem(brazier, Vector2f(0, 0), 1, "_brazier1", 1);
	initializeStaticItem(hareTrap, Vector2f(0, 0), 1, "_hareTrap1", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 1, "_fence1", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 2, "_fence2", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 3, "_fence3", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 4, "_fence4", 1);
	//

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == Monster("loadInit", Vector2f(0, 0)).getToSaveName())
			initializeDynamicItem(monster, Vector2f(posX, posY), "");
		else
			if (saveName == Deerchant("loadInit", Vector2f(0, 0)).getToSaveName())
				initializeDynamicItem(hero1, Vector2f(posX, posY), "");
			else
				if (saveName == Wolf("loadInit", Vector2f(0, 0)).getToSaveName())
					initializeDynamicItem(wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == GroundConnection("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
			initializeStaticItem(groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == Ground("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
				initializeStaticItem(ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == ForestTree("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
					initializeStaticItem(tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == Grass("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
						initializeStaticItem(grass, Vector2f(posX, posY), typeOfObject, "", 1);
					else
						if (saveName == BonefireOfInsight("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
							initializeStaticItem(bonefireOfInsight, Vector2f(posX, posY), typeOfObject, "", 1);
						else
							if (saveName == HomeCosiness("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
								initializeStaticItem(homeCosiness, Vector2f(posX, posY), typeOfObject, "", 1);
	}

	fin.close();

	//preparations for the inventory system 
	std::vector<std::reference_wrapper<HeroBag>> heroBags;
	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	for (int cnt = 0; cnt < hero->bags.size(); cnt++)
		heroBags.push_back(hero->bags[cnt]);

	inventorySystem.inventoryBounding(heroBags);
	//------------------------------------
	buildSystem.inventoryBounding(heroBags);
	buildSystem.succesInit = true;
	buildSystem.succesInit = true;

	Save();
}

void World::Save()
{
	if (staticGrid.getSize() == 0)
		return;
	std::ofstream fout("save.txt");
	fout.clear();
	std::vector<StaticObject*> staticItems = staticGrid.getItems(0, 0, width, height);
	std::vector<DynamicObject*> dynamicItems = dynamicGrid.getItems(0, 0, width, height);
	fout << dynamicItems.size() << std::endl;
	for (auto dynamicItem : dynamicItems)
	{
		fout << dynamicItem->getToSaveName() << " " << dynamicItem->getPosition().x << " " << dynamicItem->getPosition().y << std::endl;
	}
	fout << staticItems.size() << std::endl;
	for (auto staticItem : staticItems)
	{
		fout << staticItem->getToSaveName() << " " << staticItem->getType() << " " << staticItem->getPosition().x << " " << staticItem->getPosition().y << std::endl;
	}
	fout.close();
}

void World::generate(int objCount)
{
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize, microblockSize);
	bossSpawnPosition = Vector2f(width / 2, height / 2);
	auto s = float(sqrt(objCount));

	int blocksCount = ceil(width / blockSize.x) * ceil(height / blockSize.y);

	for (int i = 0; i < blocksCount; i++)
	{
		inBlockGenerate(i);
	}

	//
	initializeStaticItem(tree, Vector2f(0, 0), 1, "_forestTree1", 1);
	initializeStaticItem(spawn, Vector2f(0, 0), 1, "_spawn1", 1);
	initializeStaticItem(grass, Vector2f(0, 0), 1, "_grass1", 1);
	initializeStaticItem(ground, Vector2f(0, 0), 1, "_ground1", 1);
	initializeStaticItem(groundConnection, Vector2f(0, 0), 1, "_groundConnection1", 1);
	initializeStaticItem(bonefireOfInsight, Vector2f(0, 0), 1, "_bonefireOfInsight1", 1);
	initializeStaticItem(homeCosiness, Vector2f(0, 0), 1, "_homeCosiness1", 1);
	initializeStaticItem(mushroomStone, Vector2f(0, 0), 1, "_mushroomStone1", 1);
	initializeStaticItem(mushroomsOnStone, Vector2f(0, 0), 1, "_mushroomsOnStone1", 1);
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "_chamomile1", 1);
	initializeStaticItem(yarrow, Vector2f(0, 0), 1, "_yarrow1", 1);
	initializeStaticItem(brazier, Vector2f(0, 0), 1, "_brazier1", 1);
	initializeStaticItem(hareTrap, Vector2f(0, 0), 1, "_hareTrap1", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 1, "_fence1", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 2, "_fence2", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 3, "_fence3", 1);
	initializeStaticItem(fence, Vector2f(0, 0), 4, "_fence4", 1);

	initializeDynamicItem(hero1, Vector2f(15000, 15000), "hero1");
	initializeStaticItem(brazier, Vector2f(15500, 15000), 1, "testItem", 1);
	initializeDynamicItem(deer, Vector2f (15200, 15000), "testEnemy1");

	Save();

	//preparations for the inventory system 
	std::vector<std::reference_wrapper<HeroBag>> heroBags;
	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	for (int cnt = 0; cnt < hero->bags.size(); cnt++)
		heroBags.push_back(hero->bags[cnt]);

	inventorySystem.inventoryBounding(heroBags);
	//-------------------------------------
	buildSystem.inventoryBounding(heroBags);
	buildSystem.succesInit = true;

	return;
}

void World::inBlockGenerate(int blockIndex)
{
	if (!canBeRegenerated(blockIndex))
		return;

	const int blockType = rand() % 50;
	int genFactor, genFactorRange;

	switch (blockType)
	{
	case 1:
	{
		genFactor = 10;
		genFactorRange = 20;
		break;
	}
	case 2:
	{
		genFactor = 10;
		genFactorRange = 20;
		break;
	}
	case 3:
	{
		genFactor = 10;
		genFactorRange = 20;
		break;
	}
	default:
	{
		genFactor = 20;
		genFactorRange = 10;
		break;
	}
	}

	staticGrid.clearCell(blockIndex);

	IntRect blockTransform = IntRect(staticGrid.getPointByIndex(blockIndex).x, staticGrid.getPointByIndex(blockIndex).y, Vector2f(blockSize).x, Vector2f(blockSize).y);

	int groundIndX = staticGrid.getPointByIndex(blockIndex).x / blockSize.x;
	int groundIndY = staticGrid.getPointByIndex(blockIndex).y / blockSize.y;

	int goundType = (biomeMatrix[groundIndX][groundIndY].biomeCell);
	biomeMatrix[groundIndX][groundIndY].groundCell = new Ground("ground" + std::to_string(blockIndex), Vector2f(groundIndX * blockSize.x, groundIndY * blockSize.y), goundType);
	auto textureBounds = spriteMap["Game/worldSprites/terrainObjects/ground/ground1.png"].sprite.getGlobalBounds();
	auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	biomeMatrix[groundIndX][groundIndY].groundCell->setTextureSize(textureSize);
	staticGrid.addItem(biomeMatrix[groundIndX][groundIndY].groundCell, biomeMatrix[groundIndX][groundIndY].groundCell->getName(), biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().x, biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().y);

	auto connectionPos = biomeMatrix[groundIndX][groundIndY].groundCell->getPosition();
	auto currentType = biomeMatrix[groundIndX][groundIndY].groundCell->getType();
	auto cellSize = biomeMatrix[groundIndX][groundIndY].groundCell->getConditionalSizeUnits();

	initializeStaticItem(groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 1, "", 1);
	initializeStaticItem(groundConnection, Vector2f(connectionPos.x + blockSize.x - 1, connectionPos.y), (currentType - 1) * 4 + 2, "", 1);
	initializeStaticItem(groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 3, "", 1);
	initializeStaticItem(groundConnection, Vector2f(connectionPos.x, connectionPos.y + blockSize.y - 1), (currentType - 1) * 4 + 4, "", 1);

	//block filling
	//return;
	int saturation = rand() % 20 + 10;

	for (float i = blockTransform.left; i < blockTransform.left + blockTransform.width; i += 250)
	{
		for (float j = blockTransform.top; j < blockTransform.top + blockTransform.height; j += 250)
		{
			Vector2f randomOffset = Vector2f(rand() % 200 - 100, rand() % 200 - 100);
			auto position = Vector2f(i + randomOffset.x, j + randomOffset.y);
			if (position.x < 0)
				position.x = 0;
			if (position.y < 0)
				position.y = 0;

			int probability = rand() % 100, terrainVariety = rand() % 100;
			genFactor += rand() % genFactorRange;

			if (probability <= genFactor)
			{
				switch (blockType)
				{
				case 1:
				{
					initializeStaticItem(yarrow, position, -1, "", 0);
					break;
				}
				case 2:
				{
					initializeStaticItem(chamomile, position, -1, "", 0);
					break;
				}
				case 3:
				{
					initializeStaticItem(mugwort, position, -1, "", 0);
					break;
				}
				default:
				{
					if (terrainVariety <= 10)
						initializeStaticItem(tree, position, -1, "", 0);
					else
						if (terrainVariety <= 13)
							initializeStaticItem(rock, position, -1, "", 0);
						else
							if (terrainVariety <= 23)
								initializeStaticItem(grass, position, -1, "", 0);
							else
								if (terrainVariety <= 25)
									initializeStaticItem(stump, position, -1, "", 0);
					break;
				}
				}
			}

		}
	}
}

void World::BiomesGenerate(int offset)
{
	//initialize generation border
	Vector2i upperLeftCell = Vector2i((worldUpperLeft.x - blockSize.x * offset) / blockSize.x, (worldUpperLeft.y - blockSize.y * offset) / blockSize.y),
		bottomRightCell = Vector2i((worldBottomRight.x + blockSize.x * offset) / blockSize.x, (worldBottomRight.y + blockSize.y * offset) / blockSize.y);

	Vector2i innerUpperLeftCell = Vector2i((worldUpperLeft.x - blockSize.x) / blockSize.x, (worldUpperLeft.y - blockSize.y) / blockSize.y),
		innerBottomRightCell = Vector2i((worldBottomRight.x + blockSize.x) / blockSize.x, (worldBottomRight.y + blockSize.y) / blockSize.y);

	int inLineCount = bottomRightCell.x - upperLeftCell.x, inRawCount = bottomRightCell.y - upperLeftCell.y;
	Vector2i roughBiomeSize = Vector2i(sqrt(inLineCount), sqrt(inRawCount));

	upperLeftCell.x = std::max(0, upperLeftCell.x);
	upperLeftCell.y = std::max(0, upperLeftCell.y);
	bottomRightCell.x = std::min(int(width / blockSize.x), bottomRightCell.x);
	bottomRightCell.y = std::min(int(height / blockSize.y), bottomRightCell.y);
	//----------------------------

	for (int i = upperLeftCell.x; i <= bottomRightCell.x; i += roughBiomeSize.x)
	{
		for (int j = upperLeftCell.y; j <= bottomRightCell.y; j += roughBiomeSize.y)
		{
			int currentBiome = rand() % 4 + 1;
			for (int x = i; x <= i + roughBiomeSize.x + rand() % 2; x++)
				for (int y = j; y <= j + roughBiomeSize.y + rand() % 2; y++)
				{
					if (x >= innerUpperLeftCell.x && x <= innerBottomRightCell.x && y >= innerUpperLeftCell.y && y <= innerBottomRightCell.y)
						continue;
					biomeMatrix[x][y].biomeCell = Biomes(currentBiome);
				}
		}
	}
}

void World::perimeterGeneration(int offset)
{
	auto screenSize = Helper::GetScreenSize();

	if (focusedObject->getDirection() != STAND)
	{
		for (auto block : staticGrid.getBlocksAround(worldUpperLeft.x - blockSize.x * scaleFactor, worldUpperLeft.y - blockSize.y * scaleFactor, worldBottomRight.x + blockSize.x * scaleFactor, worldBottomRight.y + blockSize.y * scaleFactor, offset))
		{
			if (canBeRegenerated(block))
				inBlockGenerate(block);
		}
	}
}

void World::beyondScreenGeneration()
{
	if (focusedObjectBlock != Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y))
	{
		if (abs(BiomesChangeCenter.x - (focusedObject->getPosition().x / blockSize.x)) >= biomeGenerateDistance ||
			abs(BiomesChangeCenter.y - (focusedObject->getPosition().y / blockSize.y)) >= biomeGenerateDistance)
		{
			BiomesGenerate(4);
			BiomesChangeCenter = Vector2i((focusedObject->getPosition().x / blockSize.x), (focusedObject->getPosition().y / blockSize.y));
		}
		perimeterGeneration(0);
		focusedObjectBlock = Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y);
	}
}

bool World::canBeRegenerated(int blockIndex)
{
	if (blockIndex == 0)
		return false;

	for (auto item : dynamicGrid.getItems(blockIndex))
	{
		auto dynamicItem = dynamic_cast<DynamicObject*>(item);
		if (dynamicItem)
			return false;
	}

	return true;

	/*if (dynamicGrid.getItems(blockIndex).size() != 0)
		return false;

	return true;*/
}

void World::clearWorld()
{
	staticGrid.~GridList();
	dynamicGrid.~GridList();
}

void World::setTransparent(std::vector<WorldObject*> visibleItems)
{
	mouseDisplayName = "";
	Vector2f mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	float minCapacity = 10e8f, minDistance = 10e8f;

	for (auto visibleItem : visibleItems)
	{
		if (visibleItem->getName() == focusedObject->getName())
			continue;

		visibleItem->isTransparent = false;
		Vector2f itemPos = Vector2f(visibleItem->getPosition().x - visibleItem->getTextureOffset().x, visibleItem->getPosition().y - visibleItem->getTextureOffset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getConditionalSizeUnits().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getConditionalSizeUnits().y)
		{
			visibleItem->isSelected = true;
		}
		else
			visibleItem->isSelected = false;

		if (visibleItem->isTerrain && Helper::isIntersects(mousePos, IntRect(itemPos.x, itemPos.y, visibleItem->getConditionalSizeUnits().x, visibleItem->getConditionalSizeUnits().y)) ||
			Helper::getDist(mousePos, visibleItem->getPosition()) <= visibleItem->getRadius())
		{
			visibleItem->isVisibleName = true;
			float itemCapacity = visibleItem->getConditionalSizeUnits().x + visibleItem->getConditionalSizeUnits().y;
			float distanceToItemCenter;
			if (visibleItem->isTerrain)
				distanceToItemCenter = abs(mousePos.x - (itemPos.x + visibleItem->getConditionalSizeUnits().x / 2)) +
				abs(mousePos.y - (itemPos.y + visibleItem->getConditionalSizeUnits().y / 2));
			else
				distanceToItemCenter = Helper::getDist(mousePos, visibleItem->getPosition());

			if (itemCapacity < minCapacity || (itemCapacity == minCapacity && distanceToItemCenter <= minDistance))
			{
				minCapacity = itemCapacity;
				minDistance = distanceToItemCenter;

				bool isIntersect = (sqrt(pow(focusedObject->getPosition().x - visibleItem->getPosition().x, 2) + pow(focusedObject->getPosition().y - visibleItem->getPosition().y, 2)) <= (focusedObject->getRadius() + visibleItem->getRadius()));

				switch (visibleItem->tag)
				{
					case forestTreeTag:
					{
						mouseDisplayName = "Absorb";
						break;
					}
					case chamomileTag:					
					case yarrowTag:					
					case nooseTag:
					case droppedLootTag:
					{
						mouseDisplayName = "Pick up";
						break;
					}
					default:
					{
						mouseDisplayName = visibleItem->getToSaveName();
						break;
					}
				}

				selectedObject = visibleItem;
			}
		}

		if (focusedObject->getPosition().x >= itemPos.x && focusedObject->getPosition().x <= itemPos.x + visibleItem->getConditionalSizeUnits().x && focusedObject->getPosition().y >= itemPos.y && focusedObject->getPosition().y <= itemPos.y + visibleItem->getConditionalSizeUnits().y && visibleItem->isBackground == false)
		{
			visibleItem->isTransparent = true;
			if (visibleItem->transparency > 50)
				visibleItem->transparency--;
		}
		else
		{
			if (visibleItem->transparency < 100)
				visibleItem->transparency++;
		}
	}
}

bool World::fixedClimbingBeyond(Vector2f &pos)
{
	const auto screenSize = Helper::GetScreenSize();
	const auto extra = staticGrid.getBlockSize();

	if (pos.x < (screenSize.x / 2 + extra.x) * 1.5)
	{
		pos.x = (screenSize.x / 2 + extra.x) * 1.5;
		return false;
	}
	if (pos.x > width - (screenSize.x / 2 + extra.x) * 1.5)
	{
		pos.x = width - (screenSize.x / 2 + extra.x) * 1.5;
		return false;
	}
	if (pos.y < (screenSize.y / 2 + extra.y) * 1.5)
	{
		pos.y = (screenSize.y / 2 + extra.y) * 1.5;
		return false;
	}
	if (pos.y > height - (screenSize.y / 2 + extra.y) * 1.5)
	{
		pos.y = height - (screenSize.y / 2 + extra.y) * 1.5;
		return false;
	}
	return true;
}

void World::setItemFromBuildSystem()
{
	if (buildSystem.selectedObject != -1 && buildSystem.buildingPosition != Vector2f(-1, -1))
	{
		initializeStaticItem(StaticItemsIdList(buildSystem.getBuiltObjectType()), buildSystem.buildingPosition, buildSystem.getBuildType(), "", 1);

		if (buildSystem.getIsBuilding())
			buildSystem.wasPlaced();
		else
			inventorySystem.getHeldItem().content.second--;

		buildSystem.buildingPosition = Vector2f(-1, -1);
	}
}

void World::onMouseDownInteract(int currentMouseButton)
{
	Vector2i mousePos = Mouse::getPosition();
	Vector2f mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	//if (!buildSystem.getIsBuilding())
	inventorySystem.onMouseDownInteract();

	if (buildSystem.succesInit && !inventorySystem.getUsedMouse()/* && inventorySystem.getHeldItem()->first == -1*/)
		buildSystem.onMouseDownInteract(focusedObject->getPosition(), scaleFactor);

	//if (!inventorySystem.getUsedMouse())
		//buildSystem.buildHeldItem(focusedObject->getPosition(), scaleFactor);

	if (mouseDisplayName == "" || buildSystem.getUsedMouse() || inventorySystem.getUsedMouse() || currentMouseButton == 1)
		selectedObject = nullptr;

	if (currentMouseButton == 2 && !inventorySystem.getUsedMouse())
	{
		auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
		hero->onMouseDownBehavior(selectedObject, mouseWorldPos, (buildSystem.buildingPosition != Vector2f(-1, -1)));
	}

	/*Helper::drawText(std::to_string(inventorySystem.canAfford({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 6) })), 30, 200, 200, &window);	
	if (inventorySystem.canAfford({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 4) }))*/
	//auto hero = dynamic_cast<Deerchant*>(focusedObject);
	//HeroBag::putItemsIn({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 4) }, &(hero->bags));
}

void World::interact(RenderWindow& window, long long elapsedTime)
{
	scaleSmoothing();
	birthObjects();
	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();

	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto characterPosition = focusedObject->getPosition();

	Vector2f worldUpperLeft(int(characterPosition.x - screenSize.x / 2), int(characterPosition.y - screenSize.y / 2));
	Vector2f worldBottomRight(int(characterPosition.x + screenSize.x / 2), int(characterPosition.y + screenSize.y / 2));

	beyondScreenGeneration();

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);
	auto localDynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);

	const auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->heldItem = &inventorySystem.getHeldItem();

	for (auto& dynamicItem : localDynamicItems)
	{
		//interaction with other dynamic objects		
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->setTarget(*otherDynamicItem);
		}
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->behaviorWithDynamic(otherDynamicItem, elapsedTime);
		}
		//--------		
		
		//making route to the desire position
		if (dynamicItem->getMovePosition() != Vector2f(-1, -1) && dynamicItem->getRouteGenerationAbility() && dynamicItem->getCurrentAction() != jerking)
		{

				dynamicItem->resetTimeAfterNewRoute();
				dynamicItem->currentBlock = staticGrid.getMicroblockByPoint(dynamicItem->getPosition().x, dynamicItem->getPosition().y);
				staticGrid.fillLocalMatrix(dynamicItem->getMovePosition(), dynamicItem->getPosition().x - (int(screenSize.x / 2) + extra.x), dynamicItem->getPosition().y - (int(screenSize.y / 2) + extra.y), dynamicItem->getPosition().x + (int(screenSize.x / 2) + extra.x), dynamicItem->getPosition().y + (int(screenSize.y / 2) + extra.y));
				staticGrid.makeRoute(dynamicItem->getPosition(), dynamicItem->getMovePosition(), dynamicItem->getPosition().x - (int(screenSize.x / 2) + extra.x), dynamicItem->getPosition().y - (int(screenSize.y / 2) + extra.y), dynamicItem->getPosition().x + (int(screenSize.x / 2) + extra.x), dynamicItem->getPosition().y + (int(screenSize.y / 2) + extra.y));
				dynamicItem->route = staticGrid.routes[dynamicItem->getMovePosition().x / microblockSize.x][dynamicItem->getMovePosition().y / microblockSize.y];


			if (dynamicItem->route.size() >= 2)
			{
				std::pair<int, int> routeMicroblock = staticGrid.routes[dynamicItem->getMovePosition().x / microblockSize.x][dynamicItem->getMovePosition().y / microblockSize.y][0];
				std::pair<int, int> curMicroblock = std::make_pair(dynamicItem->getPosition().x / microblockSize.x, dynamicItem->getPosition().y / microblockSize.y);
				dynamicItem->changeMovePositionToRoute(Vector2f(dynamicItem->getPosition().x + (routeMicroblock.first - curMicroblock.first) * microblockSize.x, dynamicItem->getPosition().y + (routeMicroblock.second - curMicroblock.second) * microblockSize.y));
				dynamicItem->memorizedRoutePosition = dynamicItem->getMovePosition();
			}
		}

		auto intersects = false;
		auto newPosition = dynamicItem->doMove(elapsedTime), savedNewPosition = newPosition;

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->doSlip(newPosition, localStaticItems, height, elapsedTime);
		newPosition = dynamicItem->doSlipOffDynamic(newPosition, localDynamicItems, height, elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->getPosition();				

		if (!intersects)
			dynamicItem->setPosition(newPosition);
		else
			newPosition = dynamicItem->getPosition();
		dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);

		dynamicItem->behavior(elapsedTime); // the last because of position changes
	}

	//lightSystemInteract
	brightner->_emissionSprite.setPosition(Vector2f(focusedObject->getPosition().x - cameraPosition.x + Helper::GetScreenSize().x / 2, focusedObject->getPosition().y - cameraPosition.y + Helper::GetScreenSize().y / 2));

	if (focusedObject->getCurrentAction() == builds)
		setItemFromBuildSystem();
	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	buildSystem.interact();
	inventorySystem.interact(elapsedTime);
	//-------------------

	//deleting items
	for (auto item : localStaticItems)	
		if (item->getDeletePromise())		
			staticGrid.deleteItem(item->getName());
	for (auto item : localDynamicItems)
		if (item->getDeletePromise())
			dynamicGrid.deleteItem(item->getName());
	//--------------

	//saving world
	timeForNewSave += elapsedTime;
	if (timeForNewSave >= timeAfterSave)
	{
		timeForNewSave = 0;
		Save();
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	inventorySystem.wasDrawing = false;

	//shaders logic
	spiritWorldShader.setUniform("time", timer.getElapsedTime().asSeconds() / 5);
	spiritWorldShader.setUniform("level", 0);

	const auto extra = staticGrid.getBlockSize();

	auto screenSize = window.getSize();
	auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);
	cameraPosition.x += (focusedObject->getPosition().x - cameraPosition.x) * (focusedObject->getSpeed() * elapsedTime) / maxCameraDistance.x;
	cameraPosition.y += (focusedObject->getPosition().y - cameraPosition.y) * (focusedObject->getSpeed() * elapsedTime) / maxCameraDistance.y;
	worldUpperLeft = Vector2f(int(cameraPosition.x - (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y - (screenCenter.y + extra.y) / scaleFactor));
	worldBottomRight = Vector2f(int(cameraPosition.x + (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y + (screenCenter.y + extra.y) / scaleFactor));

	if (worldUpperLeft.x < 0)
		worldUpperLeft.x = 0;
	if (worldUpperLeft.y < 0)
		worldUpperLeft.y = 0;
	if (worldBottomRight.x > width)
		worldBottomRight.x = width;
	if (worldBottomRight.y > height)
		worldBottomRight.y = height;

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);
	auto localDynamicItems = dynamicGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);

	visibleBackground.clear();
	visibleTerrain.clear();
    localTerrain.clear();

	for (auto item : localStaticItems)
	{
        item->prepareSpriteNames(elapsedTime);
        for (auto& sprite : item->additionalSprites)
        {
			if (sprite.position == Vector2f(0, 0))
				sprite.position = item->getPosition();
            sprite.zCoord = item->getZCoords();
            sprite.isBackground = item->isBackground;
            if (sprite.scaleRatio == Vector2f(0, 0))
                sprite.scaleRatio = item->getScaleRatio();
			sprite.tag = item->tag;
        }
		if (item->isBackground)
			visibleBackground.insert(visibleBackground.end(), item->additionalSprites.begin(), item->additionalSprites.end());
		else
        {
			localTerrain.push_back(item);
            visibleTerrain.insert(visibleTerrain.end(), item->additionalSprites.begin(), item->additionalSprites.end());
        }
	}
    for (auto item : localDynamicItems)
    {
        item->prepareSpriteNames(elapsedTime);
        for (auto& sprite : item->additionalSprites)
        {
			if (sprite.position == Vector2f(0, 0))
				sprite.position = item->getPosition();
            sprite.zCoord = item->getZCoords();
            sprite.isBackground = item->isBackground;
            if (sprite.scaleRatio == Vector2f(0, 0))
                sprite.scaleRatio = item->getScaleRatio();
			sprite.tag = item->tag;
        }
        visibleTerrain.insert(visibleTerrain.end(), item->additionalSprites.begin(), item->additionalSprites.end());
        localTerrain.push_back(item);
    }
	sort(visibleBackground.begin(), visibleBackground.end(), cmpImgDraw);
	drawVisibleItems(window, elapsedTime, visibleBackground);

	renderLightSystem(view, window);

	sort(visibleTerrain.begin(), visibleTerrain.end(), cmpImgDraw);
	setTransparent(localTerrain);
	drawVisibleItems(window, elapsedTime, visibleTerrain);

	if (mouseDisplayName != "")
		Helper::drawText(mouseDisplayName, 30, Mouse::getPosition().x, Mouse::getPosition().y, &window);

	/*Helper::drawText(std::to_string(inventorySystem.canAfford({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 6) })), 30, 200, 200, &window);
	auto hero = dynamic_cast<Deerchant*>(focusedObject);
	if (inventorySystem.canAfford({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 4) }))	
		inventorySystem.takeItems({ std::make_pair(lootItemsIdList::noose, 1), std::make_pair(lootItemsIdList::chamomileFlower, 4) }, &(hero->bags));*/

	/*Vector2i mousePos = Mouse::getPosition();
	Vector2f mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	RectangleShape rec;
	for (auto cell : staticGrid.routes[focusedObject->getMovePosition().x / microblockSize.x][focusedObject->getMovePosition().y / microblockSize.y])
	{
		rec.setSize(Vector2f(microblockSize.x * scaleFactor, microblockSize.y * scaleFactor));
		Vector2f recPos = Vector2f(cell.first * microblockSize.x, cell.second * microblockSize.y);
		rec.setPosition((recPos.x - cameraPosition.x) * scaleFactor + Helper::GetScreenSize().x / 2, (recPos.y - cameraPosition.y) * scaleFactor + Helper::GetScreenSize().y / 2);
		rec.setFillColor(sf::Color(0, 0, 0, 125));
		window.draw(rec);
	}	*/
}

void World::runBuildSystemDrawing(RenderWindow& window, float elapsedTime)
{
	bool showBuildedObj = true;
	if (focusedObject->getSelectedTarget() != nullptr)
		showBuildedObj = focusedObject->getSelectedTarget()->tag != buildedObjectTag;

	buildSystem.draw(window, elapsedTime, spriteMap, staticGrid, scaleFactor, cameraPosition, localTerrain, showBuildedObj);
}

void World::runInventorySystemDrawing(RenderWindow& window, float elapsedTime)
{
	auto screenSize = window.getSize();
	auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	inventorySystem.drawHeroInventory(elapsedTime, window);

	if (inventorySystem.wasDrawing)
		inventorySystem.drawInventory(Vector2f(screenCenter), elapsedTime, window);
	else
		inventorySystem.resetAnimationValues();
}

void World::drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<spriteChainElement> sprites)
{
	auto screenSize = window.getSize();
	auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	std::vector<std::reference_wrapper<std::pair<lootItemsIdList, int>>> dropInventory;

	for (auto spriteChainItem : sprites)
	{
		auto sprite = (&spriteMap[spriteChainItem.path])->sprite;
		auto worldItemPosition = spriteChainItem.position;

		auto spriteLeft = float((worldItemPosition.x - cameraPosition.x - spriteChainItem.offset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - cameraPosition.y - spriteChainItem.offset.y) * scaleFactor + screenCenter.y);
		
		//sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
		sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255 * spriteChainItem.transparency / float(100)));
		if (!spriteChainItem.isBackground)
		{
			sprite.setScale(spriteChainItem.size.x / sprite.getGlobalBounds().width * scaleFactor, spriteChainItem.size.y / sprite.getGlobalBounds().height * scaleFactor * pow(scaleFactor, double(1) / 6));
			spriteTop -= (pow(scaleFactor, double(1) / 6) - 1) * sprite.getGlobalBounds().height;
		}
		else
			sprite.setScale(spriteChainItem.scaleRatio.x*scaleFactor, spriteChainItem.scaleRatio.y*scaleFactor);			
		sprite.setRotation(spriteChainItem.rotation);
		sprite.setPosition(Vector2f(spriteLeft, spriteTop));

		window.draw(sprite);
	}

	if (dropInventory.size() > 0)
	{
		inventorySystem.temporaryInventoryBounding(dropInventory);
		inventorySystem.wasDrawing = true;
	}
}

