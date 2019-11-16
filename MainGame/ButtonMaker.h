#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>

using namespace sf;

enum class ButtonTag { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6, openMenu = 7,
	nextPage = 16, previousPage = 17, bookCover = 18, bookmark = 19, pageBackground = 20, pagePattern = 21, bookmarksList = 22, sketching = 46,
	bookmarkMobs = 31, bookmarkItems = 32, bookmarkWorld = 33, bookmarkFlowers = 34, bookmarkWreathes = 35, bookmarkNightmare = 36, bookButtonTag = 40,
	bookStandTag = 41, bookGlowTag = 42, bookLightningTag = 43, hpLineTag = 44, hpFrameTag = 45, cell = 47, cellSelected = 48, iconFrame1 = 49, iconFrame2 = 50, plus = 51, makeWreath = 52,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405,
	hero1 = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109,
	heroBag = 201
};

class ButtonMaker
{
private:
	Sprite spriteDefault, spriteSelected, spritePressed;
	Texture textureDefault, textureSelected, texturePressed;
	Color originColor;
	ButtonTag tag;
	bool selected = false, isGray = false;
	Vector2f offset;
public:
	ButtonMaker();	
	void initialize(Texture &textureD, Texture &textureS, Texture &textureP, Vector2f position, Vector2f size, bool isSelectable, ButtonTag tag, Vector2f offset = {0, 0});
	void draw(RenderWindow &window);
	void bekomeGray();
	void stopBeingGray();
	bool isSelected(Vector2f mousePos);
	bool isSelectable, isActive = true;
	Vector2f getScale() { return Vector2f(spriteDefault.getScale()); }
	Vector2f getPosition() { return Vector2f(spriteDefault.getGlobalBounds().left, spriteDefault.getGlobalBounds().top); }
	Vector2f getOffset() { return offset; }
	FloatRect getGlobalBounds() { return spriteDefault.getGlobalBounds(); }
	void setPosition(Vector2f position);
	void setSize(Vector2f size);
	ButtonTag getTag() { return tag; }
	bool getGrayState() { return isGray; }
};
#endif

