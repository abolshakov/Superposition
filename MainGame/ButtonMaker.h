#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>

using namespace sf;

enum ButtonTag { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6 };

class ButtonMaker
{
private:
	Sprite sprite;
	Texture texture;
	ButtonTag tag;
	bool selected = false;
public:
	ButtonMaker();	
	void initialize(Texture &texture, Vector2f position, Vector2f size, ButtonTag tag);
	void initialize(Texture &texture, FloatRect rect, ButtonTag tag);
	void draw(RenderWindow &window);
	bool isSelected(Vector2f mousePos);
	Vector2f getScale() { return sprite.getScale(); }
	ButtonTag getTag() { return tag; }	
};
#endif

