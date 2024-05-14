#pragma once

#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include "Object.h"
#include <string>

using chop = std::pair<int, int>;

class Initialize
{
public:
	Initialize() 
	{ 
		InitFull(); 
	};
	~Initialize() = default;

	bool InitFull();
	void Render(std::vector<std::vector<Object>>& map, bool isNew, bool reverseDir);
	void HanldeLevelMsg(const std::string& msg);

private:

	int winWidth = 1200;
	int winHeight = 600;
	

	const std::string dogImg = "./Project4/Resources/dog.png";
	const std::string sheepImg = "./Project4/Resources/sheep.png";
	const std::string fenceImg = "./Project4/Resources/fence.jpg";
	const std::string barnImg = "./Project4/Resources/barn.jpg";


	SDL_Texture* LoadText(const std::string& str);
	SDL_Texture* dogText;
	SDL_Texture* fenceText;
	SDL_Texture* sheepText;
	SDL_Texture* barnText;

	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* font;
	SDL_Color textProp = { 255, 255, 255, 255 }; 
};

