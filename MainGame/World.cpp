#define _USE_MATH_DEFINES

#include "World.h"

using namespace sf;

World::World(int width, int height) : focusedObject(nullptr)
{
	timeAfterSave = 60000000;
	timeForNewSave = 0;
	blockSize = Vector2i(1000, 1000); //conditionalSizeUnits
	initSpriteMap();
	scaleFactor = getScaleFactor();

	this->width = width;
	this->height = height;

	initShaders();
	inventorySystem.init();
	buildSystem.Init(inventorySystem.getSpriteList());
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

	if (!spiritWorldShader.loadFromFile("World/Shaders/water.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
	}

	if (!distortionMap.loadFromFile("World/Shaders/noiseWater.png"))
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
	if (delta == -1 && scaleFactor > 0.75)
	{
		scaleFactor -= 0.01;
		scaleDecrease = -0.03;
	}
	else
		if (delta == 1 && scaleFactor < 1.5)
		{
			scaleFactor += 0.01;
			scaleDecrease = 0.03;
		}
}

void World::scaleSmoothing()
{
	if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
	{
		if (scaleFactor != 0.15 && scaleFactor != 1.5)
			scaleFactor += scaleDecrease;
		if (scaleDecrease < 0 && scaleFactor < 0.75)
			scaleFactor = 0.75;
		if (scaleDecrease > 0 && scaleFactor > 1.5)
			scaleFactor = 1.5;

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

bool cmpImgDraw(WorldObject* first, WorldObject* second)
{
	if (first->isBackground == true && second->isBackground == false)
		return true;
	if (first->isBackground == false && second->isBackground == true)
		return false;

	if (first->getZCoords() == second->getZCoords())
	{
		if (first->getPosition().y == second->getPosition().y)
			return first->getPosition().x < second->getPosition().x;
		return first->getPosition().y < second->getPosition().y;
	}

	return first->getZCoords() < second->getZCoords();
}

void World::initSpriteMap()
{
	std::ifstream fin(spriteNameFileDirectory);

	int objectsNumber;
	Vector2i maxSize;
	std::string name;

	while (fin >> name)
	{
		spriteMap.insert({ name, BoardSprite{} });
		auto sprite = &spriteMap[name];
		sprite->texture.loadFromFile("World/" + name);
		sprite->sprite.setTexture(sprite->texture);

		auto size = Vector2i(sprite->texture.getSize());

		if (size.x > maxSize.x)
			maxSize.x = size.x;

		if (size.y > maxSize.y)
			maxSize.y = size.y;
	}

	fin.close();

	//return maxSize;
}

void World::initLightSystem(RenderWindow &window)
{
	view = window.getDefaultView();

	contextSettings.antialiasingLevel = 8;// Включить сглаживание.

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
	//brightner->_emissionSprite.setScale(Vector2f(Helper::GetScreenSize().x / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y / light1->_emissionSprite.getTexture()->getSize().y));
	brightner->_emissionSprite.setColor(Color(240, 220, 220));
	brightner->_sourceRadius = 12;
	ls.addLight(brightner);

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

float World::getScaleFactor()
{
	auto heroHeight = Vector2i(spriteMap[heroTextureName].texture.getSize()).y;
	auto screenHeight = Helper::GetScreenSize().y;
	auto ratio = heroHeight / float(screenHeight);

	//return heroToScreenRatio / ratio;
	return 1;
}

void World::initializeStaticItem(staticItemsIdList itemClass, Vector2f itemPosition, int itemType, std::string itemName)
{
	StaticObject* item;
	std::string nameOfImage;

	switch (itemClass)
	{
	case 1:
	{
		item = new ForestTree("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/ForestTree/ForestTree";
		break;
	}
	case 2:
	{
		item = new Grass("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/grass/grass";
		break;
	}
	case 3:
	{
		item = new Spawn("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/spawn/spawn";
		break;
	}
	case 4:
	{
		item = new BonefireOfInsight("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/bonefireOfInsight/bonefireOfInsight";
		break;
	}
	case 5:
	{
		item = new HomeCosiness("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/homeCosiness/homeCosiness";
		break;
	}
	case 6:
	{
		item = new MushroomStone("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/mushroomStone/t1_";
		break;
	}
	case 7:
	{
		item = new MushroomsOnStone("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/mushroomsOnStone/t2_";
		break;
	}
	case 11:
	{
		item = new Ground("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/ground/ground";
		break;
	}
	case 12:
	{
		item = new GroundConnection("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/ground/groundConnection";
		break;
	}
	case 13:
	{
		item = new Chamomile("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/chamomile/chamomile";
		break;
	}
	case 14:
	{
		item = new Brazier("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/brazier/brazier";
		break;
	}
	case 15:
	{
		item = new Yarrow("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/yarrow/yarrow";
		break;
	}
	case 16:
	{
		item = new HareTrap("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/hareTrap/hareTrap";
		break;
	}
	default:
	{
		item = new Spawn("item", Vector2f(0, 0), -1);
		nameOfImage = "terrainObjects/spawn/spawn";
		break;
	}
	}

	int currentType = itemType == -1 
		? currentType = rand() % item->getVarietyOfTypes() + 1 
		: currentType = itemType;

	nameOfImage += std::to_string(currentType) + ".png";

	newNameId++;

	std::string name = itemName == ""
		? nameOfImage + "_" + std::to_string(newNameId)
		: itemName;

	item->setName(name);
	item->setPosition(itemPosition);
	item->setType(currentType);
	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);

	//for bias positioning
	auto spriteLeft = float((item->getPosition().x - cameraPosition.x - item->getTextureOffset().x) * scaleFactor + Helper::GetScreenSize().x / 2);
	auto spriteTop = float((item->getPosition().y - cameraPosition.y + (item->getTextureSize().y - item->getTextureOffset().y)) * scaleFactor + Helper::GetScreenSize().y / 2);	

	staticGrid.addItem(item, name, int(itemPosition.x), int(itemPosition.y));
}

void World::initializeDynamicItem(dynamicItemsIdList itemClass, Vector2f itemPosition, std::string itemName)
{
	DynamicObject* item;
	std::string nameOfImage;

	switch (itemClass)
	{
		case 1:
		{
			item = new Deerchant("item", Vector2f(0, 0));
			nameOfImage = "Hero/stand/down/1";
			focusedObject = item;
			cameraPosition = itemPosition;
			break;
		}
		case 2:
		{
			item = new Monster("item", Vector2f(0, 0));
			nameOfImage = "enemy/enemyF_0";
			break;
		}
		case 3:
		{
			item = new Wolf("item", Vector2f(0, 0));
			nameOfImage = "Wolf/stand/down/1";
			break;
		}
		default:
		{
			item = new Monster("item", Vector2f(0, 0));
			nameOfImage = "enemy/enemyF_0";
			break;
		}
	}

	nameOfImage += ".png";

	std::string name = itemName == ""
		? nameOfImage + std::to_string(dynamicGrid.getSize())
		: itemName;

	item->setName(name);
	item->setPosition(itemPosition);
	auto textureBounds = spriteMap[nameOfImage].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);

	dynamicGrid.addItem(item, name, int(itemPosition.x), int(itemPosition.y));
}

void World::Load()
{
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize);

	//
	initializeStaticItem(tree, Vector2f(0, 0), 1, "ForestTree");
	initializeStaticItem(spawn, Vector2f(0, 0), 1, "spawn");
	initializeStaticItem(grass, Vector2f(0, 0), 1, "grass");
	initializeStaticItem(ground, Vector2f(0, 0), 1, "gournd");
	initializeStaticItem(bonefireOfInsight, Vector2f(0, 0), 1, "bonefireOfInsight");
	initializeStaticItem(homeCosiness, Vector2f(0, 0), 1, "homeCosiness");
	initializeStaticItem(mushroomStone, Vector2f(0, 0), 1, "mushroomStone");
	initializeStaticItem(mushroomsOnStone, Vector2f(0, 0), 1, "mushroomsOnStone");
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "chamomile");
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "yarrow");
	initializeStaticItem(brazier, Vector2f(0, 0), 1, "brazier");
	initializeStaticItem(hareTrap, Vector2f(0, 0), 1, "hareTrap");
	//

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;
	
		if (saveName == Monster("loadInit", Vector2f(0,0)).getToSaveName())
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
			initializeStaticItem(groundConnection, Vector2f(posX, posY), typeOfObject, "");
		else
			if (saveName == Ground("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
				initializeStaticItem(ground, Vector2f(posX, posY), typeOfObject, "");
			else
				if (saveName == ForestTree("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
					initializeStaticItem(tree, Vector2f(posX, posY), typeOfObject, "");
				else
					if (saveName == Grass("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
						initializeStaticItem(grass, Vector2f(posX, posY), typeOfObject, "");
					else
						if (saveName == BonefireOfInsight("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
							initializeStaticItem(bonefireOfInsight, Vector2f(posX, posY), typeOfObject, "");
						else
							if (saveName == HomeCosiness("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
								initializeStaticItem(homeCosiness, Vector2f(posX, posY), typeOfObject, "");
	}

	fin.close();

	std::vector<std::reference_wrapper<std::pair<int, int>>> heroInventory;
	for (int cnt = 0; cnt < focusedObject->inventory.size(); cnt++)
	{
		heroInventory.push_back(focusedObject->inventory[cnt]);
	}
	inventorySystem.inventoryBounding(heroInventory);
	buildSystem.inventoryBounding(focusedObject->inventory);
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
	staticGrid = GridList<StaticObject>(this->width, this->height, blockSize);
	dynamicGrid = GridList<DynamicObject>(this->width, this->height, blockSize);
	bossSpawnPosition = Vector2f(width / 2, height / 2);
	auto s = float(sqrt(objCount));

	int blocksCount = ceil(width / blockSize.x) * ceil(height / blockSize.y);

	//
	initializeStaticItem(tree, Vector2f(0, 0), 1, "ForestTree");
	initializeStaticItem(spawn, Vector2f(0, 0), 1, "spawn");
	initializeStaticItem(grass, Vector2f(0, 0), 1, "grass");
	initializeStaticItem(ground, Vector2f(0, 0), 1, "ground");
	initializeStaticItem(groundConnection, Vector2f(0, 0), 1, "groundConnection");
	initializeStaticItem(bonefireOfInsight, Vector2f(0, 0), 1, "bonefireOfInsight");
	initializeStaticItem(homeCosiness, Vector2f(0, 0), 1, "homeCosiness");
	initializeStaticItem(mushroomStone, Vector2f(0, 0), 1, "mushroomStone");
	initializeStaticItem(mushroomsOnStone, Vector2f(0, 0), 1, "mushroomsOnStone");
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "chamomile");
	initializeStaticItem(chamomile, Vector2f(0, 0), 1, "yarrow");
	initializeStaticItem(brazier, Vector2f(0, 0), 1, "brazier");
	initializeStaticItem(hareTrap, Vector2f(0, 0), 1, "hareTrap");
	//

	for (int i = 0; i < blocksCount; i++)
	{
		inBlockGenerate(i);
	}

	//none target object
	/*dynamicGrid.addItem(new Monster("none", Vector2f(width, height)), "none", int(width), int(height));*/
	//-----------------------------------------
	//test enemy
	initializeDynamicItem(wolf, Vector2f(6000, 6100), "testEnemy1");
	initializeDynamicItem(wolf, Vector2f(5400, 5500), "testEnemy2");
	//------------------------------------------
	//initializeHero(Vector2f(3800, 4000));
	initializeDynamicItem(hero1, Vector2f(5800, 5000), "hero1");
	initializeStaticItem(hareTrap, Vector2f(6800, 6000), 1, "testItem");
	Save();

	std::vector<std::reference_wrapper<std::pair<int, int>>> heroInventory;

	for (int cnt = 0; cnt < focusedObject->inventory.size(); cnt++)
	{		
		heroInventory.push_back(focusedObject->inventory[cnt]);
	}
	inventorySystem.inventoryBounding(heroInventory);
	buildSystem.inventoryBounding(focusedObject->inventory);
	buildSystem.succesInit = true;

	return;
}

void World::inBlockGenerate(int blockIndex)
{
	if (blockIndex == 0)
		return;

	staticGrid.clearCell(blockIndex);
	IntRect blockTransform = IntRect(staticGrid.getPointByIndex(blockIndex).x, staticGrid.getPointByIndex(blockIndex).y, Vector2i(blockSize).x, Vector2i(blockSize).y);
	
	//ground
	int groundIndX = staticGrid.getPointByIndex(blockIndex).x / blockSize.x;
	int groundIndY = staticGrid.getPointByIndex(blockIndex).y / blockSize.y;
	
	int randomGroundType = rand() % Ground("item", Vector2f(0, 0), -1).getVarietyOfTypes() + 1;
	groundMatrix[groundIndX][groundIndY] = new Ground("ground" + std::to_string(blockIndex), Vector2f(groundIndX * blockSize.x, groundIndY * blockSize.y), randomGroundType);
	auto textureBounds = spriteMap["terrainObjects/ground/ground1.png"].sprite.getGlobalBounds();
	auto textureSize = Vector2i(int(textureBounds.width), int(textureBounds.height));
	groundMatrix[groundIndX][groundIndY]->setTextureSize(textureSize);
	staticGrid.addItem(groundMatrix[groundIndX][groundIndY], groundMatrix[groundIndX][groundIndY]->getName(), groundMatrix[groundIndX][groundIndY]->getPosition().x, groundMatrix[groundIndX][groundIndY]->getPosition().y);

	auto connectionPos = groundMatrix[groundIndX][groundIndY]->getPosition();
	auto currentType = groundMatrix[groundIndX][groundIndY]->getType();
	auto cellSize = groundMatrix[groundIndX][groundIndY]->getConditionalSizeUnits();
	initializeStaticItem(groundConnection, connectionPos, (currentType - 1) * 4 + 1, "");
	initializeStaticItem(groundConnection, Vector2f(connectionPos.x + cellSize.x - 1, connectionPos.y), (currentType - 1) * 4 + 2, "");
	initializeStaticItem(groundConnection, connectionPos, (groundMatrix[groundIndX][groundIndY]->getType() - 1) * 4 + 3, "");
	initializeStaticItem(groundConnection, Vector2f(connectionPos.x, connectionPos.y + cellSize.y - 1), (currentType - 1) * 4 + 4, "");

	//block filling
	Vector2f grassCellSize = Grass("", Vector2f(0, 0), 1).getConditionalSizeUnits();
	
	int saturation = rand() % 40 + 20;

	for (double i = blockTransform.left; i < blockTransform.left + blockTransform.width; i += grassCellSize.x)
	{
		for (double j = blockTransform.top; j < blockTransform.top + blockTransform.height; j += grassCellSize.y)
		{
			//grass
			auto position = Vector2f(i, j);
			/*if (i > blockTransform.left + blockTransform.width)
				position.x = blockTransform.left + blockTransform.width;
			if (j > blockTransform.top + blockTransform.height)
				position.y = blockTransform.top + blockTransform.height;
			if (blockIndex >= 0 && blockIndex <= 917)
				initializeStaticItem(grass, position, 1, "");*/

			//trees
			int probability = rand() % 100;
			if (probability <= saturation)
			{
				int greenFactor = rand() % 100;
				if (greenFactor <= 70)
					initializeStaticItem(tree, position, -1, "");
				else
					if (greenFactor <= 85)
						initializeStaticItem(chamomile, position, 1, "");
					else
						if (greenFactor <= 100)
							initializeStaticItem(yarrow, position, 1, "");
			}
			
		}
	}
}

void World::beyondScreenGenerate()
{
	auto screenSize = Helper::GetScreenSize();

	if (focusedObject->getDirection() != STAND)
	{
		for (auto block : staticGrid.getBlocksAround(worldUpperLeft.x - blockSize.x * scaleFactor, worldUpperLeft.y - blockSize.y * scaleFactor, worldBottomRight.x + blockSize.x * scaleFactor, worldBottomRight.y + blockSize.y * scaleFactor))
		{
			if (canBeRegenerated(block));
				inBlockGenerate(block);
		}
	}
}

bool World::canBeRegenerated(int blockIndex)
{
	if (dynamicGrid.getItems(blockIndex).size() != 0)
		return false;

	return true;
}

void World::ClearWorld()
{
	staticGrid.~GridList();
	dynamicGrid.~GridList();
}

Vector2f World::newSlippingPositionInCircle(DynamicObject *dynamicItem, Vector2f pos, float radius, long long elapsedTime)
{
	Vector2f motion;
	Vector2f position = dynamicItem->getPosition();

	if (dynamicItem->getDirection() == LEFT || dynamicItem->getDirection() == RIGHT)
	{
		if (dynamicItem->getDirection() == LEFT && radius - (pos.x - position.x) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (dynamicItem->getDirection() == RIGHT && radius - (position.x - pos.x) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y > pos.y)
		{
			motion = Vector2f(0, -dynamicItem->getSpeed() * elapsedTime);
			return motion;
		}
		if (position.y < pos.y)
		{
			motion = Vector2f(0, dynamicItem->getSpeed() * elapsedTime);
			return motion;
		}
	}

	if (dynamicItem->getDirection() == UP || dynamicItem->getDirection() == DOWN)
	{
		if (dynamicItem->getDirection() == UP && radius - (pos.y - position.y) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (dynamicItem->getDirection() == DOWN && radius - (position.y - pos.y) <= 5)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.x > pos.x)
		{
			motion = Vector2f(-dynamicItem->getSpeed() * elapsedTime, 0);
			return motion;
		}
		if (position.x < pos.x)
		{
			motion = Vector2f(dynamicItem->getSpeed() * elapsedTime, 0);
			return motion;
		}
	}
	if (dynamicItem->getDirection() == UPLEFT)
	{
		if (abs((pos.y - position.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (pos.y - position.y <= radius / sqrt(2))
		{
			motion = Vector2f(dynamicItem->getSpeed() * elapsedTime / sqrt(2), -dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(-dynamicItem->getSpeed() * elapsedTime / sqrt(2), dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->getDirection() == UPRIGHT)
	{
		if (abs((pos.y - position.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (pos.y - position.y <= radius / sqrt(2))
		{
			motion = Vector2f(-dynamicItem->getSpeed() * elapsedTime / sqrt(2), -dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(dynamicItem->getSpeed() * elapsedTime / sqrt(2), dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->getDirection() == DOWNLEFT)
	{
		if (abs((position.y - pos.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y - pos.y <= radius / sqrt(2))
		{
			motion = Vector2f(dynamicItem->getSpeed() * elapsedTime / sqrt(2), dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(-dynamicItem->getSpeed() * elapsedTime / sqrt(2), -dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
	}
	if (dynamicItem->getDirection() == DOWNRIGHT)
	{
		if (abs((position.y - pos.y) - radius / sqrt(2)) <= 10)
		{
			motion = Vector2f(0, 0);
			return motion;
		}
		if (position.y - pos.y <= radius / sqrt(2))
		{
			motion = Vector2f(-dynamicItem->getSpeed() * elapsedTime / sqrt(2), dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
		else
		{
			motion = Vector2f(dynamicItem->getSpeed() * elapsedTime / sqrt(2), -dynamicItem->getSpeed() * elapsedTime / sqrt(2));
			return motion;
		}
	}
	return Vector2f(-1, -1);
}

Vector2f World::newSlippingPosition(DynamicObject *dynamicItem, Vector2f pos, long long elapsedTime)
{
	Vector2f motion;
	Vector2f position = dynamicItem->getPosition();
	float speed = dynamicItem->getSpeed() / 2;
	//Vector2i pos = Vector2i((terrain->getFocus1().x + terrain->getFocus2().x) / 2, (terrain->getFocus1().y + terrain->getFocus2().y) / 2);
	if (dynamicItem->getDirection() == LEFT || dynamicItem->getDirection() == RIGHT)
	{
		if (position.y <= pos.y)
		{
			motion = Vector2f(0, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(0, speed * elapsedTime);
		return motion;
	}
	if (dynamicItem->getDirection() == UP || dynamicItem->getDirection() == DOWN)
	{
		if (position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, 0);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}
	float a = abs(pos.y - position.y);
	float b = abs(pos.x - position.x);
	float alpha = atan(b / a);
	if (dynamicItem->getDirection() == UPLEFT)
	{
		if (alpha <= M_PI / 6 || position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->getDirection() == UPRIGHT)
	{
		if (alpha <= M_PI / 6 || position.x >= pos.x)
		{
			motion = Vector2f(speed * elapsedTime, speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->getDirection() == DOWNLEFT)
	{
		if (alpha <= M_PI / 6 || position.x <= pos.x)
		{
			motion = Vector2f(-speed * elapsedTime, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(speed * elapsedTime, 0);
		return motion;
	}
	if (dynamicItem->getDirection() == DOWNRIGHT)
	{
		if (alpha <= M_PI / 6 || position.x >= pos.x)
		{
			motion = Vector2f(speed * elapsedTime, -speed * elapsedTime);
			return motion;
		}
		motion = Vector2f(-speed * elapsedTime, 0);
		return motion;
	}
	
	return Vector2f(-1, -1);
}

Vector2f World::newSlippingPositionForDynamics(DynamicObject *dynamicItem1, DynamicObject *dynamicItem2, long long elapsedTime)
{
	Vector2f motion;
	Vector2f pos1 = dynamicItem1->getPosition(), pos2 = dynamicItem2->getPosition();
	float up = dynamicItem1->getRadius() + dynamicItem2->getRadius() - (pos2.y - pos1.y);
	float right = dynamicItem1->getRadius() + dynamicItem2->getRadius() - (pos1.x - pos2.x);
	float down = dynamicItem1->getRadius() + dynamicItem2->getRadius() - (pos1.y - pos2.y);
	float left = dynamicItem1->getRadius() + dynamicItem2->getRadius() - (pos2.x - pos1.x);

	if (right <= up && right <= down && right <= left && right >= 0)
	{
		if (dynamicItem1->getDirection() == LEFT || dynamicItem1->getDirection() == UPLEFT || dynamicItem1->getDirection() == DOWNLEFT)
		{
			motion = Vector2f(0, dynamicItem1->getSpeed() * elapsedTime);
			return motion;
		}
	}

	if (down <= right && down <= up && down <= left && down >= 0)
	{
		if (dynamicItem1->getDirection() == UP || dynamicItem1->getDirection() == UPLEFT || dynamicItem1->getDirection() == UPRIGHT || dynamicItem1->getDirection() == RIGHT)
		{
			motion = Vector2f(-dynamicItem1->getSpeed() * elapsedTime, 0);
			return motion;
		}
	}

	if (left <= right && left <= down && left <= up && left >= 0)
	{
		if (dynamicItem1->getDirection() == RIGHT || dynamicItem1->getDirection() == UPRIGHT || dynamicItem1->getDirection() == DOWNRIGHT)
		{
			motion = Vector2f(0, -dynamicItem1->getSpeed() * elapsedTime);
			return motion;
		}
	}

	if (up <= right && up <= down && up <= left && up >= 0)
	{
		if (dynamicItem1->getDirection() == DOWN || dynamicItem1->getDirection() == DOWNLEFT || dynamicItem1->getDirection() == DOWNRIGHT || dynamicItem1->getDirection() == LEFT)
		{
			motion = Vector2f(dynamicItem1->getSpeed() * elapsedTime, 0);
			return motion;
		}
	}

	return Vector2f(-1, -1);
}

void World::setTransparent(std::vector<WorldObject*> visibleItems)
{
	mouseDisplayName = "";
	Vector2f mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	float minDistance = 10e8f;
	for (auto visibleItem : visibleItems)
	{
		if (visibleItem->getName() == focusedObject->getName())
			continue;		

		visibleItem->isTransparent = false;
		Vector2f itemPos = Vector2f(visibleItem->getPosition().x - visibleItem->getTextureOffset().x, visibleItem->getPosition().y - visibleItem->getTextureOffset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getTextureSize().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getTextureSize().y)
		{
			visibleItem->isSelected = true;
		}
		else
			visibleItem->isSelected = false;

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getTextureSize().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getTextureSize().y)
		{
			visibleItem->isVisibleName = true;
			//float distanceToBounds = abs(mousePos.x - visibleItem->getPosition().x) + abs(mousePos.y - visibleItem->getPosition().y);
			float distanceToBounds = visibleItem->getConditionalSizeUnits().x + visibleItem->getConditionalSizeUnits().y;

			if (distanceToBounds < minDistance)
			{
				minDistance = distanceToBounds;

				bool isIntersect = (sqrt(pow(focusedObject->getPosition().x - visibleItem->getPosition().x, 2) + pow(focusedObject->getPosition().y - visibleItem->getPosition().y, 2)) <= (focusedObject->getRadius() + visibleItem->getRadius()));

				switch (visibleItem->tag)
				{
					case forestTreeTag:
					{
						mouseDisplayName = "Absorb";
						break;
					}
					case chamomileTag:
					{
						mouseDisplayName = "Pick up";
						break;
					}
					case yarrowTag:
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

		if (focusedObject->getPosition().x >= itemPos.x && focusedObject->getPosition().x <= itemPos.x + visibleItem->getTextureSize().x && focusedObject->getPosition().y >= itemPos.y && focusedObject->getPosition().y <= itemPos.y + visibleItem->getTextureOffset().y && visibleItem->isBackground == false)
		{
			visibleItem->isTransparent = true;
			if (visibleItem->transparensy > 128)
				visibleItem->transparensy--;
		}
		else
		{
			if (visibleItem->transparensy < 255)
				visibleItem->transparensy++;
		}
	}
}

bool World::isClimbBeyond(Vector2f pos)
{
	auto screenSize = Helper::GetScreenSize();
	const auto extra = staticGrid.getBlockSize();

	if (pos.x < screenSize.x / 2 + extra.x)
		return false;
	if (pos.x > width - screenSize.x / 2 - extra.x)
		return false;
	if (pos.y < screenSize.y / 2 + extra.y)
		return false;
	if (pos.y > height - screenSize.y / 2 - extra.y)
		return false;
	return true;
}

void World::onMouseDownInteract()
{
	if (!buildSystem.getIsBuilding())
		inventorySystem.onMouseDownInteract();

	if (buildSystem.succesInit && !inventorySystem.getUsedMouse())
		buildSystem.onMouseDownInteract(focusedObject->getPosition(), scaleFactor);

	buildSystem.buildHeldItem(focusedObject->getPosition(), scaleFactor);

	if (mouseDisplayName == ""  || buildSystem.getUsedMouse() || buildSystem.getIsBuilding() || inventorySystem.getUsedMouse())
		selectedObject = nullptr;
	else
	{
		auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
		hero->onMouseDownBehavior(selectedObject);
	}

	Vector2f mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
}

void World::setItemFromBuildSystem()
{
	if (buildSystem.selectedObject != -1 && buildSystem.buildingPosition != Vector2f(-1, -1))
	{
		initializeStaticItem(staticItemsIdList(buildSystem.getBuiltObjectType()), buildSystem.buildingPosition, 1, "");
		if (buildSystem.getIsBuilding())
			buildSystem.wasPlaced();
		else
			inventorySystem.getHeldItem()->second--;

		if (inventorySystem.getHeldItem()->second <= 0)
			buildSystem.resetReadyToBuildHeldItem();
		buildSystem.buildingPosition = Vector2f(-1, -1);
	}
}

void World::interact(RenderWindow& window, long long elapsedTime)
{
	scaleSmoothing();

	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();
	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto characterPosition = focusedObject->getPosition();

	Vector2i worldUpperLeft(int(characterPosition.x - screenSize.x / 2), int(characterPosition.y - screenSize.y / 2));
	Vector2i worldBottomRight(int(characterPosition.x + screenSize.x / 2), int(characterPosition.y + screenSize.y / 2));

	if (focusedObjectBlock != staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y))
	{
		beyondScreenGenerate();
		focusedObjectBlock = staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y);
	}

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);
	auto localDynamicItems = dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);

	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	for (auto dynamicItem : localDynamicItems)
	{
		//hitInteract(*dynamicItem, elapsedTime);

		auto intersects = false;
		auto newPosition = move(*dynamicItem, elapsedTime);
		if (newPosition.x < (screenSize.x / 2 + extra.x) * 1.5)
		{
			newPosition.x = (screenSize.x / 2 + extra.x) * 1.5;
			intersects = true;
		}
		if (newPosition.x > width - (screenSize.x / 2 + extra.x) * 1.5)
		{
			newPosition.x = width - (screenSize.x / 2 + extra.x) * 1.5;
			intersects = true;
		}
		if (newPosition.y < (screenSize.y / 2 + extra.y) * 1.5)
		{
			newPosition.y = (screenSize.y / 2 + extra.y) * 1.5;
			intersects = true;
		}
		if (newPosition.y > height - (screenSize.y / 2 + extra.y) * 1.5)
		{
			newPosition.y = height - (screenSize.y / 2 + extra.y) * 1.5;
			intersects = true;
		}

		if (intersects)
		{
			dynamicItem->setPosition(newPosition);
		}

		if (!intersects)
		{
			for (auto staticItem : localStaticItems)
			{
				if (staticItem->isBackground == true)
					continue;

				dynamicItem->behaviorWithStatic(*staticItem, elapsedTime);

				auto terrain = dynamic_cast<TerrainObject*>(staticItem);
				if (!terrain)
					continue;

				if (isIntersectTerrain(newPosition, *terrain))
				{
					Vector2f terrainPos = Vector2f((terrain->getFocus1().x + terrain->getFocus2().x) / 2, (terrain->getFocus1().y + terrain->getFocus2().y) / 2);
					auto motionAfterSlipping = newSlippingPosition(dynamicItem, terrainPos, elapsedTime);
					if (motionAfterSlipping != Vector2f(-1, -1) && isClimbBeyond(Vector2f(newPosition.x + motionAfterSlipping.x, newPosition.y + motionAfterSlipping.y)))
					{
						newPosition = Vector2f(dynamicItem->getPosition().x + motionAfterSlipping.x, dynamicItem->getPosition().y + motionAfterSlipping.y);
						continue;
					}
					intersects = true;
					break;
				}
			}
		}

		//if (dynamicItem->getName() != focusedObject->getName())
		{
			for (auto otherDynamicItem : localDynamicItems)
			{
				if (otherDynamicItem == dynamicItem)
					continue;

				dynamicItem->behaviorWithDynamic(*otherDynamicItem, elapsedTime);

				if (isIntersectDynamic(*dynamicItem, newPosition, *otherDynamicItem) && dynamicItem != focusedObject)
				{
					auto motionAfterSlipping = newSlippingPositionForDynamics(dynamicItem, otherDynamicItem, elapsedTime);
					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						newPosition = Vector2f(dynamicItem->getPosition().x + motionAfterSlipping.x, dynamicItem->getPosition().y + motionAfterSlipping.y);
						continue;
					}
					intersects = true;
					break;
				}
			}
		}
		
		dynamicItem->behavior(elapsedTime);

		//if (!intersects)
		{
			dynamicItem->setPosition(newPosition);
			dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);
		}
	}

	//lightSystemInteract
	brightner->_emissionSprite.setPosition(Vector2f(focusedObject->getPosition().x - cameraPosition.x + Helper::GetScreenSize().x / 2, focusedObject->getPosition().y - cameraPosition.y + Helper::GetScreenSize().y / 2));
	
	setItemFromBuildSystem();
	buildSystem.setHeldItem(*inventorySystem.getHeldItem());
	buildSystem.interact();	

	for (auto item : localStaticItems)
	{
		if (item->getDelatePromise())
		{
			staticGrid.deleteItem(item->getName());
		}
	}

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

	//light changes
	Color currentColor;
	if (focusedObject->getCurrentWorldName() == "common")
		currentColor = commonWorldColor;
	else
		currentColor = spiritWorldColor;

	if (focusedObject->getCurrentAction() == transitionToEnotherWorld)
	{
		if (ls._ambientColor.r > 0)
			ls._ambientColor.r = currentColor.r - (currentColor.r / 6) * focusedObject->getSpriteNumber();
		if (ls._ambientColor.g > 0)
			ls._ambientColor.g = currentColor.g - (currentColor.g / 6) * focusedObject->getSpriteNumber();
		if (ls._ambientColor.b > 0)
			ls._ambientColor.b = currentColor.b - (currentColor.b / 6) * focusedObject->getSpriteNumber();
	}

	if (focusedObject->getCurrentAction() != transitionToEnotherWorld && ls._ambientColor != currentColor)
	{
		if (ls._ambientColor.r < currentColor.r)
			ls._ambientColor.r++;
		if (ls._ambientColor.g < currentColor.g)
			ls._ambientColor.g++;
		if (ls._ambientColor.b < currentColor.b)
			ls._ambientColor.b++;
	}


	const auto extra = staticGrid.getBlockSize();

	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);
	cameraPosition.x += (focusedObject->getPosition().x - cameraPosition.x) * pow(focusedObject->getSpeed(), 1.65) * elapsedTime;
	cameraPosition.y += (focusedObject->getPosition().y - cameraPosition.y) * pow(focusedObject->getSpeed(), 1.65) * elapsedTime;
	worldUpperLeft = Vector2i(int(cameraPosition.x - (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y - (screenCenter.y + extra.y) / scaleFactor));
	worldBottomRight = Vector2i(int(cameraPosition.x + (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y + (screenCenter.y + extra.y) / scaleFactor));

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

	std::vector<WorldObject*> visibleBackground, visibleTerrain;
	for (auto item : localStaticItems)
	{
		if (item->isBackground)
			visibleBackground.push_back(item);
		else
			visibleTerrain.push_back(item);
	}
	sort(visibleBackground.begin(), visibleBackground.end(), cmpImgDraw);
	drawVisibleItems(window, elapsedTime, visibleBackground);

	renderLightSystem(view, window);	
	
	auto visibleDynamicItems = std::vector<WorldObject*>(localDynamicItems.begin(), localDynamicItems.end());
	visibleTerrain.insert(visibleTerrain.end(), visibleDynamicItems.begin(), visibleDynamicItems.end());

	
	sort(visibleTerrain.begin(), visibleTerrain.end(), cmpImgDraw);
	setTransparent(visibleTerrain);
	drawVisibleItems(window, elapsedTime, visibleTerrain);

	buildSystem.draw(window, elapsedTime, spriteMap, staticGrid, scaleFactor, focusedObject->getPosition(), visibleTerrain);

	if (mouseDisplayName != "")
	{
		Helper::drawText(mouseDisplayName, 30, Mouse::getPosition().x, Mouse::getPosition().y, &window);
	}

	inventorySystem.drawHeroInventory(elapsedTime, window);

	if (inventorySystem.wasDrawing)
		inventorySystem.drawInventory(Vector2f(screenCenter), elapsedTime, window);
	else
		inventorySystem.resetAnimationValues();

	Vector2f mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	/*Helper::drawText(std::to_string(staticGrid.getPointByIndex(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)).x), 30, 200, 200, &window);
	Helper::drawText(std::to_string(staticGrid.getPointByIndex(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)).y), 30, 350, 200, &window);*/
	//Helper::drawText(std::to_string(focusedObject->getTargetPosition().x), 30, 200, 400, &window);
	//Helper::drawText(std::to_string(spriteMap[heroTextureName].texture.getSize().y), 30, 500, 300, &window);
	/*Helper::drawText(std::to_string(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)), 30, 200, 400, &window);*/
	//int groundIndX = staticGrid.getPointByIndex(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)).x / blockSize.x;
	//int groundIndY = staticGrid.getPointByIndex(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)).y / blockSize.y;
	//std::string blockName = staticGrid.getItemByName("ground" + std::to_string(staticGrid.getIndexByPoint(focusedObject->getPosition().x, focusedObject->getPosition().y)))->getName();
	/*for (int cnt = 0; cnt < staticGrid.getBlocksAround(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y).size(); cnt++)
	{
		Helper::drawText(std::to_string(staticGrid.getBlocksAround(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y)[cnt]), 30, 100 * cnt, 500, &window);
	}*/
	
}	

void World::drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<WorldObject*> visibleItems)
{
	auto screenSize = window.getSize();
	auto screenCenter = Vector2i(screenSize.x / 2, screenSize.y / 2);

	std::vector<std::reference_wrapper<std::pair<int, int>>> dropInventory;

	for (auto worldItem : visibleItems)
	{
		auto worldItemPosition = worldItem->getPosition();
		auto worldTextureOffset = worldItem->getTextureOffset();
		worldTextureOffset = Vector2i(worldTextureOffset.x * worldItem->getConditionalSizeUnits().x / worldItem->getTextureSize().x, worldTextureOffset.y * worldItem->getConditionalSizeUnits().y / worldItem->getTextureSize().y);
		auto worldTextureSize = worldItem->getTextureSize();

		auto spriteLeft = float((worldItemPosition.x - cameraPosition.x - worldTextureOffset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - cameraPosition.y + (worldItem->getConditionalSizeUnits().y - worldTextureOffset.y)) * scaleFactor + screenCenter.y);
		//auto spriteRight = float(spriteLeft + worldTextureSize.x * scaleFactor);
		//auto spriteBottom = float(spriteTop + worldTextureSize.y * scaleFactor);

		auto sprite = (&spriteMap[worldItem->getSpriteName(elapsedTime)])->sprite;

		sprite.setPosition(Vector2f(spriteLeft, spriteTop));

		sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);

		if (!worldItem->isBackground)
			sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor*pow(scaleFactor, double(1) / 6));
		else
			sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor/* / pow(scaleFactor, double(1) / 8)*/);

		//sprite.setScale(worldItem->getScaleRatio().x*scaleFactor, worldItem->getScaleRatio().y*scaleFactor);

		//set transparent
		sprite.setColor(Color(sprite.getColor().r * worldItem->getBrightness() / 100, sprite.getColor().g * worldItem->getBrightness() / 100, sprite.getColor().b * worldItem->getBrightness() / 100, worldItem->transparensy * worldItem->getBrightness() / 100));
		//sprite.setColor(Color(sprite.getColor().r * 0, sprite.getColor().g * 0, sprite.getColor().b * 0, worldItem->transparensy));
		sprite.setRotation(worldItem->getRotation());

		if (focusedObject->getCurrentWorldName() == "spirit"/* && !worldItem->isTransparent*/)
			window.draw(sprite, &spiritWorldShader);
		else
			window.draw(sprite);

		if (worldItem->getInventoryVisibility())
		{
			for (int cnt = 0; cnt < worldItem->inventory.size(); cnt++)
			{
				dropInventory.push_back(worldItem->inventory[cnt]);
			}
		}	
	}

	if (dropInventory.size() > 0)
	{
		inventorySystem.temporaryInventoryBounding(dropInventory);
		inventorySystem.wasDrawing = true;
	}
}

Vector2f World::move(DynamicObject& dynamicObject, long long elapsedTime)
{
	if (dynamicObject.getDirection() == STAND)
		return dynamicObject.getPosition();

	auto position = dynamicObject.getPosition();
	auto moveOffset = dynamicObject.getMoveOffset();

	if (moveOffset != Vector2f(-1, -1))
	{
		position.x += moveOffset.x * elapsedTime;
		position.y += moveOffset.y * elapsedTime;

		return position;
	}

	auto angle = dynamicObject.getDirection() * M_PI / 180;	

	position.x = float(position.x + dynamicObject.getSpeed() * cos(angle) * elapsedTime);
	position.y = float(position.y - dynamicObject.getSpeed() * sin(angle) * elapsedTime);

	return position;
}

bool World::isIntersectTerrain(Vector2f newPosition, const TerrainObject& terrain) const
{
	auto f1 = terrain.getFocus1();
	auto f2 = terrain.getFocus2();
	//Vector2f position = dynamic.getPosition();
	Vector2f position = newPosition;
	return (sqrt((position.x - f1.x)*(position.x - f1.x) + (position.y - f1.y)*(position.y - f1.y)) + sqrt((position.x - f2.x)*(position.x - f2.x) + (position.y - f2.y)*(position.y - f2.y))/* - dynamic.radius*/) <= terrain.getEllipseSize();
}

bool World::isIntersectDynamic(DynamicObject& dynamic1, Vector2f newPosition, DynamicObject& dynamic2) const
{
	//Vector2f position1 = dynamic1.getPosition();
	Vector2f position1 = newPosition;
	Vector2f position2 = dynamic2.getPosition();
	return (abs(position1.x - position2.x) <= (dynamic1.getRadius() + dynamic2.getRadius()) && abs(position1.y - position2.y) <= (dynamic1.getRadius() + dynamic2.getRadius()));
	//return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= dynamic1.radius + dynamic2.radius;
}

