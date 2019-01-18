#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>

using namespace sf;

enum ButtonTag { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6, nextPage = 16, previousPage = 17,bookCover = 18, bookmark = 19, pageBackground = 20, yarrowBlockTag = 21, hareBlockTag = 22, heroBookBlockTag = 23, chamomileBlockTag = 24, criticismBlockTag = 25, myPeaceBlockTag = 26, wreathBlockTag = 27, nightmareBlockTag = 28, bookmarkMobs = 31, bookmarkItems = 32, bookmarkHerbs = 33, bookmarkWreathes = 34, bookmarkNightmare = 35 };

class ButtonMaker
{
private:
	Sprite sprite;
	Texture texture;
	ButtonTag tag;
	bool selected = false;	
public:
	ButtonMaker();	
	void initialize(Texture &texture, Vector2f position, Vector2f size, bool isSelectable, ButtonTag tag);
	void initialize(Texture &texture, FloatRect rect, bool isSelectable, ButtonTag tag);
	void draw(RenderWindow &window);
	bool isSelected(Vector2f mousePos);
	bool isSelectable;
	Vector2f getScale() { return Vector2f(sprite.getScale()); }
	Vector2f getPosition() { return Vector2f(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top); }
	FloatRect getGlobalBounds() { return sprite.getGlobalBounds(); }
	void setPosition(Vector2f position) { sprite.setPosition(position.x, position.y); }
	ButtonTag getTag() { return tag; }	
};
#endif

