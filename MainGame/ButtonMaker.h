#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>

using namespace sf;

enum ButtonTag { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6, openMenu = 7,
	nextPage = 16, previousPage = 17,bookCover = 18, bookmark = 19, pageBackground = 20, pagePattern = 21, bookmarksList = 22,
	heroBookBlockTag = 23, chamomileBlockTag = 24, wreathBlockTag = 27, nightmareBlockTag = 28,
	yarrowBlockTag = 29, hareBlockTag = 30, bookmarkMobs = 31, bookmarkItems = 32, bookmarkHerbs = 33, bookmarkWreathes = 34, bookmarkNightmare = 35, bookButtonTag = 40,
	bookStandTag = 41, bookGlowTag = 42, bookLightningTag = 43, hpLineTag = 44, hpFrameTag = 45 };

class ButtonMaker
{
private:
	Sprite spriteDefault, spriteSelected, spritePressed;
	Texture textureDefault, textureSelected, texturePressed;
	ButtonTag tag;
	bool selected = false;
public:
	ButtonMaker();	
	void initialize(Texture &textureD, Texture &textureS, Texture &textureP, Vector2f position, Vector2f size, bool isSelectable, ButtonTag tag);
	void draw(RenderWindow &window);
	bool isSelected(Vector2f mousePos);
	bool isSelectable, isActive = true;
	Vector2f getScale() { return Vector2f(spriteDefault.getScale()); }
	Vector2f getPosition() { return Vector2f(spriteDefault.getGlobalBounds().left, spriteDefault.getGlobalBounds().top); }
	FloatRect getGlobalBounds() { return spriteDefault.getGlobalBounds(); }
	void setPosition(Vector2f position);
	void setSize(Vector2f size);
	ButtonTag getTag() { return tag; }	
};
#endif

