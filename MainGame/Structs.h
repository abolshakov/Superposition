#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

struct boardObject
{
	string name;
	string type;
	int xPos;
	int yPos;
	int xSize;
	int ySize;
};

#endif