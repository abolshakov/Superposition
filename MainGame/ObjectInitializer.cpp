#include "ObjectInitializer.h"
#include "Helper.h"

#include "Rock.h"
#include "Ground.h"
#include "GroundConnection.h"
#include "DroppedLoot.h"
#include "Grass.h"
#include "Fog.h"
#include "Spawn.h"
#include "BonefireOfInsight.h"
#include "HomeCosiness.h"
#include "MushroomStone.h"
#include "MushroomsOnStone.h"
#include "Chamomile.h"
#include "Yarrow.h"
#include "Mugwort.h"
#include "Fern.h"
#include "Brazier.h"
#include "Totem.h"
#include "HareTrap.h"
#include "Fence.h"
#include "Stump.h"
#include "ForestTree.h"
#include "WreathTable.h"

ObjectInitializer::ObjectInitializer()
{
}


ObjectInitializer::~ObjectInitializer()
{
}

int ObjectInitializer::newNameId = 0;

StaticObject* ObjectInitializer::initializeStaticItem(
	Tag itemClass,
	Vector2f itemPosition,
	int itemType,
	std::string itemName,
	int count,
	Vector2f cameraPosition,
	float scaleFactor,
	std::unordered_map<std::string, BoardSprite>* spriteMap,
	std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = nullptr;

	switch (itemClass)
	{
		case Tag::tree:
		{
			item = new ForestTree("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::grass:
		{
			item = new Grass("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::spawn:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::bonefireOfInsight:
		{
			item = new BonefireOfInsight("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::homeCosiness:
		{
			item = new HomeCosiness("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::mushroomStone:
		{
			item = new MushroomStone("item", Vector2f(0, 0), -1);
		}
		case Tag::mushroomsOnStone:
		{
			item = new MushroomsOnStone("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::ground:
		{
			item = new Ground("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::groundConnection:
		{
			item = new GroundConnection("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::chamomile:
		{
			item = new Chamomile("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::brazier:
		{
			item = new Brazier("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::wreathTable:
		{
			item = new WreathTable("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::yarrow:
		{
			item = new Yarrow("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fern:
		{
			item = new Fern("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::hareTrap:
		{
			item = new HareTrap("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::rock:
		{
			item = new Rock("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fence:
		{
			item = new Fence("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::stump:
		{
			item = new Stump("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::droppedLoot:
		{
			item = new DroppedLoot("item", Vector2f(0, 0), -1, count);
			break;
		}
		case Tag::mugwort:
		{
			item = new Mugwort("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::totem:
		{
			item = new Totem("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fog:
		{
			item = new Fog("item", Vector2f(0, 0), -1);
			break;
		}
		default:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
	}

	int currentType = itemType == -1
		? currentType = rand() % item->getVarietyOfTypes() + 1
		: currentType = itemType;

	newNameId++;

	item->setPosition(Vector2f(itemPosition));
	item->setType(currentType);
	item->prepareSpriteNames(0);
	FloatRect textureBounds = FloatRect(0, 0, 0, 0);
	if (spriteMap->count(item->additionalSprites[0].path) > 0)
	textureBounds = spriteMap->at(item->additionalSprites[0].path).sprite.getGlobalBounds();
	const auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);
	const std::string name = itemName.empty()
		? item->additionalSprites[0].path + "_" + std::to_string(newNameId)
		: itemName;
	item->setName(name);
	if (!inventory.empty())
		item->inventory = inventory;

	//for bias positioning
	auto spriteLeft = float((item->getPosition().x - cameraPosition.x - item->getTextureOffset().x) * scaleFactor + Helper::GetScreenSize().x / 2);
	auto spriteTop = float((item->getPosition().y - cameraPosition.y + (item->getConditionalSizeUnits().y - item->getTextureOffset().y)) * scaleFactor + Helper::GetScreenSize().y / 2);
	return item;
}