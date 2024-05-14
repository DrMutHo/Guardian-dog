#pragma once

#include <SDL.h>

const unsigned int width = 50;
const unsigned int height = 50;

enum States
{
	INVALID = -1,
	FENCES,
	DOGS,
	SHEEPS,
	GRASS
};

struct Object
{
	SDL_Rect pos;
	States st;
	bool onBarn;
	Object(unsigned int r, unsigned int c, States st, bool onBarn);
};

