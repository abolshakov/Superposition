#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

struct boardObject
{
	std::string name;
	std::string type;
	int xPos;
	int yPos;
	int xSize;
	int ySize;
};

#endif