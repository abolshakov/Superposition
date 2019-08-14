#ifndef WREATHDRAFT_H
#define WREATHDRAFT_H

#include <SFML/Graphics.hpp>
#include "ButtonMaker.h"
#include "WorldObject.h"

using namespace sf;

struct DraftInfo
{
	std::vector<std::pair<Tag, std::pair<int, int>>> plants;
	std::vector<int> rings;
	Tag id;
};

class WreathDraft
{
private:
	DraftInfo originalSetup;
public:
	WreathDraft();
	~WreathDraft();
	void init(DraftInfo originalSetup);
	DraftInfo getOriginalSetup() const { return originalSetup; }

	DraftInfo currentSetup;
};

#endif;