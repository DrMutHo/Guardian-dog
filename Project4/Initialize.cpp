#include <SDL_image.h>
#include "Initialize.h"
#include "Object.h"

bool Initialize::InitFull()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Guardian Dog", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, winWidth, winHeight);
	SDL_SetRenderDrawColor(renderer, 0, 127, 19, 255);
	TTF_Init();
	std::string fontFile = "./Project4/STIXGeneral.ttf";
	font = TTF_OpenFont(fontFile.c_str(), 90);
	

	return true;
}

//UTILS
SDL_Texture* Initialize::LoadText(const std::string& str)
{
	SDL_Surface* surface = IMG_Load(str.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

//render yard
void Initialize::Render(std::vector<std::vector<Object>>& map, bool isNew, bool reverseDir)
{	
	//Nếu là lần đầu vào game
	if (isNew)
	{
		dogText = LoadText(dogImg);
		fenceText = LoadText(fenceImg);
		sheepText = LoadText(sheepImg);
		barnText = LoadText(barnImg);
	}

	SDL_RenderClear(renderer);
	for (std::vector<Object>& curRow : map)
	{
		for (Object& curObj : curRow)
		{
			
			if (curObj.st == DOGS)
			{
				//Nếu đổi chiều chuyển động
				if (reverseDir)
				{
					SDL_RendererFlip flipPlayer = SDL_FLIP_HORIZONTAL;
					SDL_RenderCopyEx(renderer, dogText, NULL,
						&curObj.pos, 0, NULL, flipPlayer);
				}
				else
				{
					SDL_RenderCopy(renderer, dogText, NULL, &curObj.pos);
				}
			}
			else if (curObj.st == FENCES)
			{
				SDL_RenderCopy(renderer, fenceText, NULL, &curObj.pos);
			}
			else if (curObj.st == SHEEPS)
			{
				SDL_RenderCopy(renderer, sheepText, NULL, &curObj.pos);
			}
			
			else if (curObj.st == GRASS && curObj.onBarn)
			{
				SDL_RenderCopy(renderer, barnText, NULL, &curObj.pos);
			}
			else
				SDL_RenderFillRect(renderer, &curObj.pos);
		}
	}
	SDL_RenderPresent(renderer);
}

//Xử lý message qua màn và phá đảo trò chơi
void Initialize::HanldeLevelMsg(const std::string& msg)
{
	SDL_RenderClear(renderer);

	SDL_Surface* blendedText = TTF_RenderText_Blended(font, msg.c_str(), textProp);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, blendedText);
	SDL_FreeSurface(blendedText);

	SDL_Rect textRect = { winWidth / 4, winHeight / 4, 50, 50 };
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);

	if (msg == "YOU WON!")
		textRect.x -= 50;
	else
		textRect.x -= 150;

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
}


