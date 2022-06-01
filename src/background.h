#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <type_traits>
#include <iterator>
#include <initializer_list>
#include "screen.h"

template <int height, int width>
struct Background
{
	bool grid = true;
	int tileSize;
	int xShift;
	int yShift;

	Background(int tSize, int xShft, int yShft) : tileSize{tSize}, xShift{xShft}, yShift{yShft} {}
};

#endif
